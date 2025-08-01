#pragma once

#include <libgen.h>
#include <limits.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace frb  // fieldro_bot
{

/**
 * @brief      실행 파일 경로로 현재 디렉토리 설정
 * @note
 */
inline void set_current_dir_to_executable_path()
{
  char    result[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
  if(count != -1)
  {
    char* path = dirname(result);
    if(chdir(path) != 0)
    {
      std::cerr << "Failed to change directory to executable path" << std::endl;
    }
  }
}

/**
/* @brief  현재 날짜 또는 시간을 반환
/* @param  std::string s 확인하고자 하는 시간+날짜, 날짜 등의 옵션
/* @return std::string 결과 시간 표현식
*/
inline std::string getCurrentDateTime(std::string s)
{
  timeval tv;
  tm      tstruct;
  char    buf[80];

  gettimeofday(&tv, NULL);           // 현재 시간 확인
  tstruct = *localtime(&tv.tv_sec);  // 구해진 시간을 LocalTime으로 변경

  if(s == "now")
  {
    strftime(buf, sizeof(buf), "%Y-%m-%d %X",
             &tstruct);  // Year-Month-Day Hour Format
  }
  else if(s == "date")
  {
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);  // Year-Month-Day Format
  }
  else if(s == "datetime")
  {
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H",
             &tstruct);  // Year-Month-Day_Hour(24)  Format
  }
  else if(s == "sec")
  {
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H:%M:%S",
             &tstruct);  // Year-Month-Day_Hour(24)  Format
  }
  else if(s == "msec")
  {
    char timebuf[80];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d_%H:%M:%S",
             &tstruct);  // Year-Month-Day_Hour(24):Second
    // sprintf(buf,"%s.%03ld", timebuf, tv.tv_usec/1000);                  //
    // .millisecond
    int   needed      = snprintf(nullptr, 0, "%s.%03d", timebuf, static_cast<int>(tv.tv_usec / 1000));
    char* dynamic_buf = new char[needed + 1];
    snprintf(dynamic_buf, needed + 1, "%s.%03d", timebuf, static_cast<int>(tv.tv_usec / 1000));
    delete[] dynamic_buf;
  }
  else
  {
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H:%M:%S",
             &tstruct);  // Year-Month-Day_Hour(24)  Format
  }
  return std::string(buf);
}

/**
/* @brief  console에서 user input을 받아서 string으로 반환
/* @param  void
/* @return 입력받은 문자열 (/n 제거된)
*/
inline std::string get_user_input()
{
  std::string input;
  std::getline(std::cin, input);
  return input;
}

/**
/* @brief  user input을 받아서 처리
/* @param  int32_t a_timeout : timeout value - microsecond
/* @return select method return value  (0:timeout), (1:input), (-1:error)
*/
inline int32_t check_std_in(int32_t a_timeout)
{
  fd_set         rfds;
  struct timeval timeout;

  FD_ZERO(&rfds);    // 0으로 초기화
  FD_SET(0, &rfds);  // 파일 디스크립터 0 (stdin) 설정

  timeout.tv_sec  = a_timeout / 1000000;  // timeout 설정
  timeout.tv_usec = a_timeout % 1000000;  // timeout 설정

  return select(1, &rfds, nullptr, nullptr, &timeout) > 0;
}

/**
/* @brief  string을 첫번째 구분자를 기준으로 명령어 목적지와, 명령줄로 분리
/* @param  string str  변환할 문자열 (명령어 목적지)
/* @return 목적지와, 명령줄로 분리된 문자열
*/
inline std::vector<std::string> find_command_target(std::string str, char operator_char)
{
  std::string              target  = "";
  std::string              command = "";
  size_t                   pos     = std::string::npos;
  std::vector<std::string> rets;
  rets.clear();

  if(str.find("\n") != std::string::npos) str.replace(str.find("\n"), 1, "");

  if(str.size() > 0)
  {
    pos = str.find(operator_char);
    if(pos != std::string::npos)
    {
      target  = str.substr(0, pos);
      command = str.substr(pos + 1, str.size() - pos);
    }
    else
    {
      target = str;
    }
  }

  rets.push_back(target);
  rets.push_back(command);

  return rets;
}

/**
/* @brief  string을 첫번째 구분자를 기준으로 command, option으로 분리
/* @param  string str  변환할 문자열
/* @return command, option으로 분리된 문자열
*/
inline std::vector<std::string> separate_cmd(std::string str, char operator_char)
{
  std::string              cmd    = "";  // str;
  std::string              option = "";
  size_t                   pos    = std::string::npos;
  std::vector<std::string> rets;
  rets.clear();

  if(str.find("\n") != std::string::npos) str.replace(str.find("\n"), 1, "");

  if(str.size() > 0)
  {
    pos = str.find(operator_char);
    if(pos != std::string::npos)
    {
      cmd    = str.substr(0, pos);
      option = str.substr(pos + 1, str.size() - pos);
    }
    else
    {
      cmd = str;
    }
  }
  rets.push_back(cmd);
  rets.push_back(option);
  return rets;
}

/**
/* @brief  string을 구분자를 기준으로 각각의 option으로 분리
/* @param  string str  변환할 문자열
/* @return 각option으로 분리된 문자열
*/
inline std::vector<std::string> separate_option(std::string str, char operator_char)
{
  std::vector<std::string> tokens;
  std::stringstream        ss(str);
  std::string              string_buffer;

  if(str.find("\n") != std::string::npos) str.replace(str.find("\n"), 1, "");

  tokens.clear();
  while(getline(ss, string_buffer,
                operator_char))  // 공백문자를 기준으로 문자열을 자른다.
  {
    if(string_buffer.size() > 0)  // 문자열이 비어있지 않다면
    {
      tokens.push_back(string_buffer);  // tokens에 추가
    }
  }
  return tokens;
}

// string을 구분자를 기준으로 명령어와 명령줄로 구분을 하고 명령줄을 '='을
// 기준으로 분리하여 map에 저장
inline std::string parsing_command_map(std::string input_string, std::map<std::string, int32_t>& command_map)
{
  command_map.clear();
  std::vector<std::string> input = separate_cmd(input_string, ' ');

  if(input.size() < 1)
  {
    return "";
  }
  else if(input.size() > 1)
  {
    command_map.clear();

    // option string을 공백으로 구분하여 저장
    std::vector<std::string> option = separate_option(input[1], ' ');

    // 공백으로 구분된 option string을 '='으로 구분하여 map에 저장
    for(int i = 0; i < option.size(); ++i)
    {
      std::vector<std::string> map_value = separate_option(option[i], '=');

      // map에 저장된 option string을 명령어와 Value로 구분하여 저장
      // 명령어만 있는 경우 Value 값에는 0 설정하여 저장
      if(map_value.size() == 1)
      {
        command_map.insert(make_pair(map_value[0], 0));
      }
      else if(map_value.size() == 2)
      {
        command_map.insert(make_pair(map_value[0], atoi(map_value[1].c_str())));
      }
    }

    std::string ret_string = input[0];
    input.clear();
    return ret_string;
  }
  return "";
}

// string을 구분자를 기준으로 명령어와 명령줄로 구분을 하고 명령줄을 '='을
// 기준으로 분리하여 map에 저장
inline std::string seperate_key_value(std::string input_string, std::map<std::string, int32_t>& command_map)
{
  command_map.clear();
  std::vector<std::string> map_value = separate_option(input_string, '=');

  if(map_value.size() < 1)
  {
    return "";
  }
  else if(map_value.size() > 1)
  {
    // map에 저장된 option string을 명령어와 Value로 구분하여 저장
    // 명령어만 있는 경우 Value 값에는 0 설정하여 저장
    if(map_value.size() == 1)
    {
      command_map.insert(make_pair(map_value[0], 0));
    }
    else if(map_value.size() == 2)
    {
      command_map.insert(make_pair(map_value[0], atoi(map_value[1].c_str())));
    }

    std::string ret_string = map_value[0];
    map_value.clear();
    return ret_string;
  }
  return "";
}

// Safe Delete Macro
template <typename T>
void safe_delete(T& p)
{
  delete p;
  p = nullptr;
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/**
/* @brief  			인자없는 delaycall 함수
/* @param  			시간 경과 후 호출 되어야 하는 콜백 함수
/* @return 			void
/* @note        template<typename... Args> inline void delay_call 가 있는데 인자업슨
/*              callback을 만든 이유는 호출 할 때 너무 인자 지시자가 너무 길어져서이다.
/*              templete 함수로 하게 되면std::function<void()>([this](){this->init_check();})
/*              이렇게 호출이 복잡하다 그래서 인자없는 콜백은 별도로
*/
inline void delay_call(int32_t millisecond, std::function<void()> callback)
{
  // thread를 사용해서 비동기 작업 실행
  std::thread(
      [millisecond, callback]()
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
        callback();
      })
      .detach();
}

template <typename... Args>
inline void delay_call(int32_t millisecond, std::function<void(Args...)> callback, Args... args)
{
  std::thread([millisecond, callback, args...]()
              {
        std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
        callback(args...); })
      .detach();
}
}  // namespace frb