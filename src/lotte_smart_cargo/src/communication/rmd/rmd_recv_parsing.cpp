#include "rmd.h"

int32_t frb::Rmd::recv_position(const std::array<uint8_t, 8>& packet)
{
  int32_t motor_position        = (packet[7] << 24) | (packet[6] << 16) | (packet[5] << 8) | packet[4];
  double  actual_motor_position = static_cast<double>(motor_position) * 0.01f;

  if(_position_current == 0.0)
  {
    _position_current = actual_motor_position;
    set_position();
  }
  _logger->push_log_info("PROC", "Current Position", std::to_string(motor_position));
}

bool frb::Rmd::recv_hw_status(const std::array<uint8_t, 8>& packet)
{
  bool motor_stall      = false;
  bool low_voltage      = false;
  bool overvoltage      = false;
  bool overcurrent      = false;
  bool power_overrun    = false;
  bool speeding         = false;
  bool over_temperature = false;

  int32_t error_status = (packet[7] << 8) | packet[6];

  motor_stall      = (error_status & 0x0002) != 0;
  low_voltage      = (error_status & 0x0004) != 0;
  overvoltage      = (error_status & 0x0008) != 0;
  overcurrent      = (error_status & 0x0010) != 0;
  power_overrun    = (error_status & 0x0040) != 0;
  speeding         = (error_status & 0x0100) != 0;
  over_temperature = (error_status & 0x1000) != 0;

  _logger->push_log_info("PROC", "Hardware Status", std::to_string(error_status));
}

int32_t frb::Rmd::recv_current_speed(const std::array<uint8_t, 8>& packet)
{
  int32_t motor_speed = 0;
  motor_speed         = (packet[5] << 8) | packet[4];

  _logger->push_log_info("PROC", "Current Speed", std::to_string(motor_speed));
}