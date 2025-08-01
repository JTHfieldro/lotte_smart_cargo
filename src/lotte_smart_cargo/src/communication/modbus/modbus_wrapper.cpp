#include "modbus_wrapper.h"

std::mutex frb::ModbusWrapper::_mtx;

frb::ModbusWrapper::ModbusWrapper(const std::string& config_path, frb::Logger* logger, frb::ModbusType type)
    : _config_path(config_path), _logger(logger), _type(type)
{
  _modbus                = nullptr;  // modbus 객체 초기화
  _is_connected          = false;    // modbus 연결 여부 초기화
  _last_try_connect_time = 0;        // 마지막 modbus 연결 시도 시간 초기화
  _last_retry_time       = 0;

  switch(_type)
  {
    case frb::ModbusType::Rs232: break;
    case frb::ModbusType::Rtu: load_option(); break;
    case frb::ModbusType::Tcp: load_option(); break;
  }
}

frb::ModbusWrapper::~ModbusWrapper()
{
  switch(_type)
  {
    case frb::ModbusType::Rs232: break;
    case frb::ModbusType::Rtu: disconnect_modbus_rtu(); break;
    case frb::ModbusType::Tcp: disconnect_modbus_tcp(); break;
  }
}

bool frb::ModbusWrapper::connect_check(int32_t slave_id)
{
  switch(_type)
  {
    case frb::ModbusType::Rs232: return false;
    case frb::ModbusType::Rtu: return modbus_rtu_connect_check(slave_id);
    case frb::ModbusType::Tcp: return modbus_tcp_connect_check();
  }
  return false;
}

bool frb::ModbusWrapper::is_connect() { return _is_connected; }

void frb::ModbusWrapper::load_option()
{
  try
  {
    // file open
    std::ifstream yaml_file(_config_path);
    YAML::Node    yaml = YAML::Load(yaml_file);
    yaml_file.close();

    // data read
    std::string config_name = yaml["config_name"].as<std::string>();
    _logger->push_log_info("PROC", config_name);

    switch(_type)
    {
      case frb::ModbusType::Rs232: break;
      case frb::ModbusType::Rtu:
      {
        _retry_count = yaml["modbus"]["retry_count"].as<int32_t>();
        _retry_turm  = yaml["modbus"]["retry_turm"].as<int32_t>();
        _device_name = yaml["rtu"]["device"].as<std::string>();
        _baudrate    = yaml["rtu"]["baudrate"].as<int32_t>();
        _stop_bit    = yaml["rtu"]["stop_bit"].as<int32_t>();
        _slave_id    = yaml["rtu"]["slave_id"].as<std::int32_t>();
        _time_out    = yaml["rtu"]["time_out"].as<std::int32_t>();
        break;
      }
      case frb::ModbusType::Tcp:
      {
        _ip          = yaml["tcp"]["ip"].as<std::string>();
        _port        = yaml["tcp"]["port"].as<int32_t>();
        _retry_count = yaml["modbus"]["retry_count"].as<int32_t>();
        _retry_turm  = yaml["modbus"]["retry_turm"].as<int32_t>();
        break;
      }
    }
  }
  catch(YAML::Exception& e)
  {
    _logger->push_log_error("PROC", "YAML Exception", e.what());
  }
  catch(std::exception& e)
  {
    _logger->push_log_error("PROC", "Standard Exception", e.what());
  }
  catch(...)
  {
    _logger->push_log_error("PROC", "Unknown Exception", "");
  }
}