#include "rs232_comm.h"

bool frb::Rs232Comm::connect_check()
{
  if(_serial_port == nullptr) return false;
  if(_is_connected) return true;
  if(_retry_count <= 0) return false;  // 재시도 횟수를 모두 소진

  if(!try_connect_232()) return false;
  else return true;
}

bool frb::Rs232Comm::try_connect_232()
{
  if(_serial_port == nullptr) return false;
  if((_last_try_connect_time + _retry_turm) > std::time(nullptr)) return false;  // turm 이내에 시도한 적이 있다.
  if(_retry_count <= 0)
  {
    _logger->push_log_error("PROC", "Retry Count", "0");
    return false;
  }  // 재시도 횟수를 모두 소진했다.

  _logger->push_log_info("PROC", "Retry Count", std::to_string(_retry_count));
  _last_try_connect_time = std::time(nullptr);
  _retry_count--;

  if(!connect_232())
  {
    _logger->push_log_error("PROC", "Fail to RS232 Connection");
    return false;
  }

  _is_connected = true;
  return true;
}

bool frb::Rs232Comm::connect_232()
{
  if(_serial_port == nullptr) return false;

  try
  {
    if(_serial_port->isOpen())
    {
      _serial_port->close();
    }
    // Port SettingE
    _serial_port->setBaudrate(_baudrate);
    _serial_port->setBytesize(serial::eightbits);
    _serial_port->setFlowcontrol(serial::flowcontrol_none);
    _serial_port->setParity(serial::parity_none);
    _serial_port->setStopbits(serial::stopbits_one);

    _serial_port->open();

    _logger->push_log_info("PROC", "Success to RS232 Connection");
    return true;
  }
  catch(const serial::IOException& e)
  {
    _logger->push_log_error("PROC", "Failed to open serial port: ", e.what());
    return false;
  }
}

void frb::Rs232Comm::disconnect_232()
{
  if(_serial_port->isOpen()) _serial_port->close();
}