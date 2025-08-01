#include "rmd.h"

void frb::Rmd::set_home()
{
  _control_mode = frb::RmdDefine::MWriteOffsetPosCurrent;
  _send_packet_callback(set_packet_cmd(_control_mode));
  _logger->push_log_info("PROC", "Set Home Position to Current Position");
}

void frb::Rmd::set_position()
{
  _position_home = _position_current;
  _position_min  = _position_home;
  _position_max  = _position_home - _position_gap;

  _logger->push_log_info("PROC", "Set Temporary Home to Current Position");
  _logger->push_log_info("PROC", "Home", std::to_string(_position_home));
  _logger->push_log_info("PROC", "Max", std::to_string(_position_max));
  _logger->push_log_info("PROC", "Min", std::to_string(_position_min));
}
