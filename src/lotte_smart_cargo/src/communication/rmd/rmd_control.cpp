#include "rmd.h"

void frb::Rmd::control_speed(const int32_t rpm)
{
  _control_mode = frb::RmdDefine::CCtrlSpeed;
  _send_packet_callback(set_packet_speed(rpm * 600));
  _logger->push_log_info("PROC", "Control Speed Mode", std::to_string(rpm) + " [rpm]");
}

void frb::Rmd::control_abs_position(const int32_t target_position)
{
  _control_mode = frb::RmdDefine::CCtrlPosition;
  _send_packet_callback(set_packet_position(target_position));
  _logger->push_log_info("PROC", "Control ABS Position Mode", std::to_string(target_position) + " [Unit]");
}

void frb::Rmd::control_inc_position(const int32_t target_position)
{
  _control_mode = frb::RmdDefine::CCtrlIncPosition;
  _send_packet_callback(set_packet_inc_position(target_position));
  _logger->push_log_info("PROC", "Control INC Position Mode", std::to_string(target_position) + " [Unit]");
}

void frb::Rmd::control_stop()
{
  _control_mode = frb::RmdDefine::MotorCtrlStop;
  _send_packet_callback(set_packet_cmd(frb::RmdDefine::MotorCtrlStop));
  _logger->push_log_info("PROC", "Motor Stop");
}

void frb::Rmd::control_shutdown()
{
  _control_mode = frb::RmdDefine::MotorCtrlShutdown;
  _send_packet_callback(set_packet_cmd(frb::RmdDefine::MotorCtrlShutdown));
  _logger->push_log_info("PROC", "Motor Shutdown");
}

void frb::Rmd::control_brake(const bool flag)
{
  if(flag)
  {
    _control_mode = frb::RmdDefine::MotorCtrlBreakOn;
    _send_packet_callback(set_packet_cmd(frb::RmdDefine::MotorCtrlBreakOn));
    _logger->push_log_info("PROC", "Brake is Hold");
  }
  else
  {
    _control_mode = frb::RmdDefine::MotorCtrlBreakOff;
    _send_packet_callback(set_packet_cmd(frb::RmdDefine::MotorCtrlBreakOff));
    _logger->push_log_info("PROC", "Brake is Release");
  }
}

void frb::Rmd::control_reset()
{
  _control_mode = frb::RmdDefine::MotorCtrlReset;
  _send_packet_callback(set_packet_cmd(frb::RmdDefine::MotorCtrlReset));
  _logger->push_log_info("PROC", "Motor Reset");
}