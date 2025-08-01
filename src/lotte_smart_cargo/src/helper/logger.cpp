
#include "logger.h"

frb::Logger* frb::Logger::instance = nullptr;

/**
/* @brief  			Logger객체를 singleton pattern으로 획득하기 위한 함수
*/
frb::Logger* frb::Logger::get(const std::string& custom_folder)
{
  if(instance == nullptr)
  {
    instance = new frb::Logger(custom_folder);
  }
  return instance;
}

void frb::Logger::destroy()
{
  if(instance)
  {
    Logger* temp = instance;
    instance     = nullptr;  // 먼저 nullptr 설정
    delete temp;             // 그 다음 삭제
  }
}

/**
/* @brief  			frb::Logger 생성자
*/
frb::Logger::Logger(const std::string& custom_folder)
{
  char path[PATH_MAX];

  _custom_folder  = custom_folder;
  _component      = custom_folder;
  _component_size = _component.size();

  size_t component_set_width = 20;
  size_t diff_size           = component_set_width - _component.size();
  size_t left_set_width      = diff_size / 2;
  size_t right_set_width     = diff_size - left_set_width;

  _component = std::string(left_set_width, ' ') + _component + std::string(right_set_width, ' ');

  std::transform(_component.begin(), _component.end(), _component.begin(),
                 [](unsigned char c)
                 { return std::toupper(c); });

  if(getcwd(path, PATH_MAX))
  {
    std::stringstream ss;
    ss << "log/" << _custom_folder;
    // _dir = std::string(path) + "./log/" + _custom_folder;
    _dir = ss.str();
  }
  else
  {
    std::cerr << "getcwd error" << std::endl;
    throw std::runtime_error("Failed to get current directory");
  }

  _exit_flag = false;
  _thread    = std::thread(&frb::Logger::update, this);
}

/**
/* @brief  			frb::Logger 객체 소멸자
/* @note        자체적으로 thread가 동작하고 있으므로 thread를 멈춰야 한다.
*/
frb::Logger::~Logger()
{
  _exit_flag = true;
  _log_condition.notify_one();

  if(_thread.joinable())
  {
    _thread.join();
  }
}

/**
/* @brief  			파일로 남기고자 하는 로그 기록
/* @param
/* @return
*/
void frb::Logger::push_log(const std::string& time_operator, const std::string& msg)
{
  // lock_guard scope 때문에 아래의 {} 풀면 안된다
  {
    std::lock_guard<std::mutex> guard(_list_mutex);
    _log_list.push_back(msg);
  }
  _log_condition.notify_one();
}

/** @brief Log Format 지정 함수
 * @param level
 * @param action
 * @param target
 * @param value
 * @note level[INFO, WARN, ERROR]
 * @note action[PUB, SUB, PROC]
 */
void frb::Logger::push_log_format(const std::string& level, const std::string& action,
                                  const std::string& target, const std::string& value)
{
  if(instance == nullptr) return;

  std::stringstream ss, ss_color;

  // 시간 태그 생성 및 저장
  std::string tag       = "";
  std::string file_path = make_filepath("sec", tag);

  // 레벨에 따른 색상 설정
  std::string level_color;
  if(level == "INFO") level_color = frb::GREEN;
  else if(level == "WARN") level_color = frb::YELLOW;
  else if(level == "ERROR") level_color = frb::RED;
  else level_color = frb::RESET;

  std::string action_color;
  if(action == "PUB") action_color = frb::RED;
  else if(action == "SUB") action_color = frb::BLUE;
  else if(action == "SRV") action_color = frb::MAGENTA;
  else if(action == "PROC") action_color = frb::GREEN;
  else action_color = frb::RESET;

  if(target == "Start")
  {
    ss_color << "\n\n";
    ss << "\n\n";
  }

  // 색상이 포함된 버전 (콘솔용)
  ss_color << tag << '\t' << level_color << "[" << std::left << std::setw(5) << level << "]" << frb::RESET << " "
           << "[" << std::left << _component << "]" << frb::RESET << " " << action_color
           << std::left << std::setw(5) << action << frb::RESET << "| " << std::left << std::setw(37) << target;

  // 색상이 없는 버전 (파일용)
  ss << tag << '\t' << "[" << std::left << std::setw(5) << level << "] " << "[" << std::left
     << _component << "] " << std::left << std::setw(5) << action << "| " << std::left << std::setw(37)
     << target;

  if(!value.empty())
  {
    ss_color << "-> " << std::left << std::setw(10) << value;
    ss << "-> " << std::left << std::setw(10) << value;
  }

  std::cout << "" << '\t' << ss_color.str() << std::endl;  // 콘솔에 기록
  push_log(std::string("sec"), ss.str());
}

void frb::Logger::push_log_info(const std::string& action, const std::string& target, const std::string& value)
{
  push_log_format("INFO", action, target, value);
}
void frb::Logger::push_log_warn(const std::string& action, const std::string& target, const std::string& value)
{
  push_log_format("WARN", action, target, value);
}
void frb::Logger::push_log_error(const std::string& action, const std::string& target, const std::string& value)
{
  push_log_format("ERROR", action, target, value);
}

void frb::Logger::change_component(const std::string& hw) { _component = hw; }

/**
/* @brief  			frb::Logger 객체의 thread 함수
/* @note        _log_condition.notify_one() 되면 _log_list에 추가된 message를 로그로 기록
/* @attention   _log_condition.notify_one() 조건과
                !_log_list.empty() || _exit_flag 조건이 참이 되어야 wait가 깨어난다.
*/
void frb::Logger::update()
{
  while(!_exit_flag)
  {
    std::unique_lock<std::mutex> listLock(_list_mutex);

    std::call_once(_clean_directory, std::bind(&frb::Logger::cleanup_old_directory, this));

    // 통보가 오길 기다린다
    _log_condition.wait(listLock, [this]
                        { return !_log_list.empty() || _exit_flag; });

    while(!_log_list.empty())
    {
      std::string logMessage = _log_list.front();
      _log_list.pop_front();
      listLock.unlock();

      // 파일로 기록
      std::lock_guard<std::mutex> fileLock(_file_mutex);
      std::string                 tag       = "";
      std::string                 file_path = make_filepath("sec", tag);

      // 파일경로 중간의 디렉토리들이 없으면 생성
      create_directory(file_path);
      // create_directory(common_path);

      std::ofstream ofs(file_path, std::ios_base::out | std::ios_base::app);
      if(ofs.is_open())
      {
        // ofs << _tag << '\t' << logMessage << std::endl;  // 파일에 기록
        ofs << logMessage << std::endl;  // 파일에 기록
      }
      else
      {
        std::cerr << "Failed to open log file" << std::endl;
      }

      // 통보가 올 때까지 다시 lock
      listLock.lock();
    }
  }
}

/**
/* @brief  			기록하여야 하는 file path를 만들기 (파일명 포함)
/* @param  			std::string mode : 기록 모드
/* @param       std::string& log_time : 각 로그기록 앞에 붙여야 할 tag (일반적으로 시간)
/* @return 			std::string : 파일명이 포함된 전체 경로
*/
std::string frb::Logger::make_filepath(const std::string& mode, std::string& log_time)
{
  auto     now       = std::chrono::system_clock::now();
  auto     in_time_t = std::chrono::system_clock::to_time_t(now);
  std::tm* now_tm    = std::localtime(&in_time_t);

  // File Path
  std::stringstream file_path;
  // file_path << _dir << "/" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d") <<
  // "/";   // Dir명 만들기
  file_path << _dir << "/";
  file_path << std::setfill('0') << std::setw(4) << now_tm->tm_year + 1900 << "-";
  file_path << std::setfill('0') << std::setw(2) << now_tm->tm_mon + 1 << "-";
  file_path << std::setfill('0') << std::setw(2) << now_tm->tm_mday << "/";
  file_path << std::setfill('0') << std::setw(2) << now_tm->tm_hour << ".log";  // 파일명 만들기

  // log 기록 시간 만들기
  std::stringstream logtime;
  if(mode == "sec")
  {
    logtime << std::put_time(std::localtime(&in_time_t), "%X");
    log_time = logtime.str();
  }
  else
  {
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    logtime << std::setfill('0') << std::setw(3) << now_ms.count();
    log_time = logtime.str();
  }

  return file_path.str();
}

/**
/* @brief  			디렉토리 생성 함수
/* @param  			std::string& path : 디렉토리명 포함 경로
/* @return 			생성 여부
/* @note        1. 생성된 디렉토리에는 모든 권한을 다 준다.
/*              2. boost lib를 이용하여 경로포함 파일명을 입력하면 중간에 없는 Directory를
/*                 생성 해주는 방법으로 변경 하였음
*/
bool frb::Logger::create_directory(std::string& file_path)
{
  // if (mkdir(path.c_str(), 0777) == -1)
  // {
  //   std::cerr << "Error: " << strerror(errno) << std::endl;
  //   return false;
  // }
  // return true;
  boost::filesystem::path dir(file_path);
  if(!boost::filesystem::exists(dir.parent_path()))
  {
    if(boost::filesystem::create_directories(dir.parent_path()))
    {
      chmod(dir.parent_path().c_str(), 0777);  // 모든 권한 부여
      std::cout << "Directory created: " << dir.parent_path() << std::endl;
      return true;
    }
    else
    {
      return false;
    }
  }

  return true;
}

/**
 * @brief      현재 일자 기준으로 _duration만큼 경과된 Log Folder 제거
 * @note
 */
void frb::Logger::cleanup_old_directory() { remove_old_directory(_dir, _duration); }

/**
 * @brief      특정 경로에서 기한이 경과된 폴더 삭제하는 헬퍼 함수
 * @param[in]  탐색할 경로
 * @param[in]  경과 기준 일자
 * @note
 */
void frb::Logger::remove_old_directory(const std::string& base_path, const uint8_t& duration)
{
  try
  {
    boost::filesystem::path log_path(base_path);

    if(!boost::filesystem::exists(log_path))
    {
      return;
    }

    // 기본 경로의 모든 항목을 순회
    for(const auto& entry : boost::filesystem::directory_iterator(log_path))
    {
      if(boost::filesystem::is_directory(entry))
      {
        // YYYY-MM-DD 형식의 폴더인지 확인
        if(is_directory_older_than(entry.path(), duration))
        {
          // 폴더와 그 내용을 모두 삭제
          boost::filesystem::remove_all(entry.path());
          std::cout << "Removed old log folder: " << entry.path() << std::endl;
        }
      }
    }
  }
  catch(const boost::filesystem::filesystem_error& e)
  {
    std::cerr << "Error cleaning up logs: " << e.what() << std::endl;
  }
}

/**
 * @brief      폴더 이름에서 날짜를 파싱해 경과 여부 반환
 * @param[in]  검토 대상 폴더 경로
 * @param[in]  경과 기준 일자
 * @return     경과 여부
 * @note
 */
bool frb::Logger::is_directory_older_than(const boost::filesystem::path& folder_path, const uint8_t& days)
{
  try
  {
    std::string folder_name = folder_path.filename().string();

    // YYYY-MM-DD 형식인지 확인
    if(folder_name.length() != 10 || folder_name[4] != '-' || folder_name[7] != '-')
    {
      return false;
    }
    // 폴더 이름에서 날짜 파싱
    int16_t year  = std::stoi(folder_name.substr(0, 4));
    int16_t month = std::stoi(folder_name.substr(5, 2));
    int16_t day   = std::stoi(folder_name.substr(8, 2));

    // 날짜 유효성 검사
    if(year < 1900 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31)
    {
      std::cerr << "Invalid date in folder name: " << folder_name << std::endl;
      return false;
    }

    std::tm folder_tm = {};
    folder_tm.tm_year = year - 1900;
    folder_tm.tm_mon  = month - 1;
    folder_tm.tm_mday = day;
    folder_tm.tm_hour = 12;  // 정오로 설정하여 일광절약시간 문제 방지
    folder_tm.tm_min  = 0;
    folder_tm.tm_sec  = 0;

    // 현재 시간 가져오기
    auto now        = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    // 폴더 시간을 time_t로 변환
    auto folder_time = std::mktime(&folder_tm);
    if(folder_time == -1)
    {
      std::cerr << "Failed to convert time for folder: " << folder_name << std::endl;
      return false;
    }

    // 날짜 차이 계산 (초 단위를 일 단위로 변환)
    const int seconds_per_day = 60 * 60 * 24;
    double    diff_days       = std::difftime(now_time_t, folder_time) / seconds_per_day;

    return diff_days > days;
  }
  catch(const std::exception& e)
  {
    std::cerr << "Error checking folder age: " << e.what() << std::endl;
    return false;
  }
}
