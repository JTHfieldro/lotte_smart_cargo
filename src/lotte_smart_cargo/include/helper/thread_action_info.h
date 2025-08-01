#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "yaml-cpp/yaml.h"

namespace frb  // fieldro_bot
{

class ThreadActionInfo
{
public:
  ThreadActionInfo(std::string file_name);
  virtual ~ThreadActionInfo();

protected:
  ThreadActionInfo() {}  // 일반 생성자 Block

public:
  bool        _active;  // thread 동작 여부
  int64_t     _hz;      // thread 동작 속도 (Hz)
  int64_t     _sleep;   // thread sleep 시간 (ms) _thread_hz에 따라 계산
  std::thread _thread;  // thread 객체

protected:
  void load_option(std::string file_name);
};
}