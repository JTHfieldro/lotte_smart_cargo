#include "rs232_comm.h"

bool frb::Rs232Comm::send_message(const std::vector<uint8_t>& msg)
{
  std::lock_guard<std::mutex> lock(_lock);
  try
  {
    _serial_port->write(msg);
    // _logger->push_log_info("PROC", "Success to Send Message");
    return true;
  }
  catch(const std::exception& e)
  {
    _logger->push_log_error("PROC", "Fail to Send Message");
    return false;
  }
  return false;
}

void frb::Rs232Comm::send_message(const std::string& msg)
{
  std::lock_guard<std::mutex> lock(_lock);
  try
  {
    _serial_port->write(msg);
    // _logger->push_log_info("PROC", "Success to Send Message");
    return true;
  }
  catch(const std::exception& e)
  {
    _logger->push_log_error("PROC", "Fail to Send Message");
    return false;
  }
  return false;
}

std::vector<uint8_t> frb::Rs232Comm::receive_message()
{
  std::lock_guard<std::mutex> lock(_lock);
  std::vector<uint8_t>        buffer(6);
  buffer.clear();

  try
  {
    for(int i = 0; i < 6; ++i) _serial_port->read(buffer);
    // _logger->push_log_info("PROC", "Success to Read Message");
  }
  catch(const std::exception& e)
  {
    _logger->push_log_error("PROC", "Fail to Read Message");
  }

  return buffer;
}

std::string frb::Rs232Comm::receive_response()
{
  std::lock_guard<std::mutex> lock(_lock);
  std::string                 response;
  uint8_t                     byte;
  size_t                      bytes_read;

  try
  {  // 종료 문자(])가 나올 때까지 읽기
    while(true)
    {
      bytes_read = _serial_port->read(&byte, 1);  // 한 바이트씩 읽기
      if(bytes_read == 0) continue;               // 타임아웃 또는 읽을 데이터 없음

      response += static_cast<char>(byte);
      if(byte == ']') break;  // 종료 문자 확인
    }
  }
  catch(const std::exception& e)
  {
    _logger->push_log_error("PROC", "Fail to Read Message");
  }

  return response;
}