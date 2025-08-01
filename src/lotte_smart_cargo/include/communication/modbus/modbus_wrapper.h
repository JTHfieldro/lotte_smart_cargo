#pragma once

#include <modbus/modbus.h>
#include <smart_cargo_libs/helper/global_helper.h>
#include <smart_cargo_libs/helper/logger.h>
#include <yaml-cpp/yaml.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>

#include "modbus_wrapper_define.h"

namespace frb  // fieldro_bot
{

class ModbusWrapper
{
private:
  ModbusWrapper() {};

public:
  ModbusWrapper(const std::string& config_path, frb::Logger* logger, frb::ModbusType type);
  ~ModbusWrapper();

public:
  bool connect_check(int32_t slave_id = 1);
  bool is_connect();

  // TCP
  int32_t read_data_tcp(int32_t address, int32_t read_len, uint8_t* dest);
  int32_t write_data_tcp(int32_t address, int32_t status);

  // RTU
  void    set_slave_id_rtu(const int16_t id);
  int32_t read_data_rtu(int32_t id, int32_t address, int32_t read_len, uint16_t* dest);
  int32_t write_data_rtu(int32_t id, int32_t address, int32_t status);
  int32_t write_datas_rtu(int32_t id, int32_t address, int32_t size, const uint16_t* data);

protected:
  frb::Logger*      _logger;
  modbus_t*         _modbus;
  frb::ModbusType   _type;
  static std::mutex _mtx;  // 공유를 위한 정적 mutex 객체
  std::string       _config_path;

  // TCP
  std::string _ip;
  int32_t     _port;

  // RTU
  std::string _device_name;
  int32_t     _baudrate;
  int16_t     _stop_bit;
  int16_t     _slave_id;
  int32_t     _time_out;

  bool        _is_connected;
  std::time_t _last_try_connect_time;
  std::time_t _last_retry_time;
  int32_t     _retry_count;
  int32_t     _retry_turm;

protected:
  void load_option();

  // TCP
  bool try_connect_modbus_tcp();
  bool connect_modbus_tcp();
  void disconnect_modbus_tcp();
  bool modbus_tcp_connect_check();

  // RTU
  bool try_connect_modbus_rtu(int32_t slave_id);
  bool connect_modbus_rtu(int32_t slave_id);
  void disconnect_modbus_rtu();
  bool modbus_rtu_connect_check(int32_t slave_id);
};
}  // namespace frb