
#pragma once

// C 표준 라이브러리
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
// C++ 표준 라이브러리
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
// 외부 라이브러리
#include <boost/filesystem.hpp>

namespace frb  // fieldro_bot
{

// ANSI 색상 코드 정의
const std::string RED     = "\033[0;31m";
const std::string GREEN   = "\033[0;32m";
const std::string YELLOW  = "\033[0;33m";
const std::string BLUE    = "\033[0;34m";
const std::string MAGENTA = "\033[0;35m";
const std::string RESET   = "\033[0m";

class Logger
{
public:
  Logger(const std::string& custom_folder = "default");
  ~Logger();

public:
  static Logger* get(const std::string& custom_folder = "default");
  static void    destroy();

  void push_log_info(const std::string& action, const std::string& target, const std::string& value = "");
  void push_log_warn(const std::string& action, const std::string& target, const std::string& value = "");
  void push_log_error(const std::string& action, const std::string& target, const std::string& value = "");
  void change_component(const std::string& hw);

private:
  static Logger* instance;

  const uint8_t _duration = 30;

  std::string             _custom_folder;
  std::string             _component;
  size_t                  _component_size;
  std::mutex              _file_mutex;
  std::mutex              _list_mutex;
  std::condition_variable _log_condition;
  std::list<std::string>  _log_list;
  std::string             _dir;
  std::thread             _thread;
  bool                    _exit_flag;
  std::once_flag          _clean_directory;

  Logger(const Logger&)            = delete;  // 복사 생성자 삭제
  Logger& operator=(const Logger&) = delete;  // 대입 연산자 삭제

  void update();  // Logger 객체 동작 thread

  bool        create_directory(std::string& file_path);                       // 디렉토리 생성
  std::string make_filepath(const std::string& mode, std::string& log_time);  // file path 생성

  void push_log(const std::string& time_operator, const std::string& msg);
  void push_log_format(const std::string& level, const std::string& action, const std::string& target, const std::string& value = "");

  void cleanup_old_directory();
  void remove_old_directory(const std::string& base_path, const uint8_t& duration);
  bool is_directory_older_than(const boost::filesystem::path& folder_path, const uint8_t& days);
};
}  // namespace frb