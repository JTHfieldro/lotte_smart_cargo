#include "rs232_comm.h"

frb::Rs232Comm::Rs232Comm(const std::string& config_path, frb::Logger* logger)
    : _config_path(config_path), _logger(logger)
{
  initialize();
}

frb::Rs232Comm::~Rs232Comm()
{
  disconnect_232();
  safe_delete(_serial_port);
}

void frb::Rs232Comm::initialize()
{
  load_option();
  _last_try_connect_time = std::time(nullptr);
  _is_connected          = false;

  _serial_port = new serial::Serial(_device, _baudrate, serial::Timeout::simpleTimeout(1000));
}

void frb::Rs232Comm::load_option()
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

    _retry_turm  = yaml["rs232"]["retry_turm"].as<int32_t>();
    _retry_count = yaml["rs232"]["retry_count"].as<int32_t>();

    if(_hw == "PIO_MAINT")
    {
      // Maint option
      _device    = yaml["maint"]["device"].as<std::string>();
      _baudrate  = yaml["maint"]["baudrate"].as<int32_t>();
      _data_bits = yaml["maint"]["databits"].as<int32_t>();
      _parity    = yaml["maint"]["parity"].as<int32_t>();
      _stop_bits = yaml["maint"]["stopbits"].as<int32_t>();
    }
    else if(_hw == "PIO_SERIAL")
    {
      // Serial option
      _device    = yaml["serial"]["device"].as<std::string>();
      _baudrate  = yaml["serial"]["baudrate"].as<int32_t>();
      _data_bits = yaml["serial"]["databits"].as<int32_t>();
      _parity    = yaml["serial"]["parity"].as<int32_t>();
      _stop_bits = yaml["serial"]["stopbits"].as<int32_t>();
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
    _logger->push_log_error("PROC", "Unknown Exception");
  }
}
