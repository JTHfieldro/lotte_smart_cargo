#pragma once

// #include <SerialPort.h>
#include <serial/serial.h>
#include <smart_cargo_libs/helper/global_helper.h>
#include <smart_cargo_libs/helper/logger.h>

#include <string>

#include "yaml-cpp/yaml.h"

namespace frb  // fieldro_bot
{

class Rs232Comm
{
private:
  Rs232Comm() {}

public:
  Rs232Comm(const std::string& config_path, frb::Logger* logger);
  ~Rs232Comm();

public:
  bool is_connected() const { return (_serial_port->isOpen()) && _is_connected; }
  bool connect_check();

  bool                 send_message(const std::vector<uint8_t>& msg);
  bool                 send_message(const std::string& msg);
  std::vector<uint8_t> receive_message();
  std::string          receive_response();

protected:
  frb::Logger* _logger;
  std::mutex   _lock;
  std::string  _config_path;

  // SerialPort* _serial_port;
  serial::Serial* _serial_port;
  std::string     _device;
  int32_t         _baudrate;
  int32_t         _data_bits;
  int32_t         _parity;
  int32_t         _stop_bits;

  std::time_t _last_try_connect_time;  // 최근 재연결 시간
  bool        _is_connected;           // dxl 연결 여부
  int32_t     _retry_turm;             // 재연결 시도 간격
  int32_t     _retry_count;            // 재연결 시도 횟수

protected:
  void initialize();
  void load_option();  // motor can 변수 호출

  bool try_connect_232();
  bool connect_232();
  void disconnect_232();
};
};  // namespace frb