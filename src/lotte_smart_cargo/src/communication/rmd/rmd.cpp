#include "rmd.h"

frb::Rmd::Rmd(const std::string& config_path, frb::Logger* logger, std::function<void(std::array<uint8_t, 8>)> send_packet_callback)
    : _config_path(config_path), _logger(logger), _send_packet_callback(send_packet_callback)
{
  initialize();
}

frb::Rmd::~Rmd()
{
  _thread_info->active = false;
  safe_delete(_thread_info);
}

void frb::Rmd::control(const std::string& str)
{
  std::vector<std::string> input = find_command_target(str, ' ');

  if(input[0] == "all_finish")
  {
  }
}

void frb::Rmd::update()
{
  while(_thread_info->_active)
  {
    pop_recv_packet();
    // thread 무한 독점을 방지하기 위한 sleep
    std::this_thread::sleep_for(std::chrono::milliseconds(_thread_info->_sleep));
  }
}

void frb::Rmd::initialize()
{
  _can_send_id       = 0;
  _can_recv_id       = 0;
  _control_mode      = frb::RmdDefine::NextPacketNone;
  _rpm_ctrl_speed    = 0;
  _rpm_ctrl_position = 0;
  _position_max      = 0.0f;
  _position_min      = 0.0f;
  _position_gap      = 0.0f;
  _position_home     = 0.0f;
  _position_current  = 0.0f;

  load_option();

  _thread_info = new ThreadActionInfo(_config_path);

  // thread 정보 설정
  _thread_info->_active = true;
  _thread_info->_thread = std::thread(std::bind(&frb::Rmd::update, this));
}

void frb::Rmd::load_option()
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

    // Option 정보
    _can_send_id       = yaml["can"]["send_id"].as<int16_t>();
    _can_recv_id       = _can_send_id + 256;
    _rpm_ctrl_speed    = yaml["rmd"]["rpm_ctrl_speed"].as<int32_t>() * 600;
    _rpm_ctrl_position = yaml["rmd"]["rpm_ctrl_position"].as<int32_t>() * 6;
    _position_max      = yaml["rmd"]["position_max"].as<double>();
    _position_min      = yaml["rmd"]["position_min"].as<double>();
    _position_gap      = yaml["rmd"]["position_gap"].as<double>();
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
