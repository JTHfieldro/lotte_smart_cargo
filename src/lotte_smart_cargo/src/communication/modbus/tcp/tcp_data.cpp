#include "modbus_wrapper.h"

int32_t frb::ModbusWrapper::read_data_tcp(int32_t address, int32_t read_len, uint8_t* dest)
{
  // modbus 연결 되어있지 않으면 return
  if(!_is_connected) return frb::to_int(frb::CommRes::Fail);

  // data 읽기
  std::lock_guard<std::mutex> lock(_mtx);
  size_t                      read_bits = modbus_read_bits(_modbus, address, read_len, dest);

  // 읽어들인 Data 길이 확인
  if(read_bits != read_len)
  {
    _logger->push_log_error("PROC", "modbus_read_bits fail !!!", "");
    _logger->push_log_error("PROC", "Error Number", modbus_strerror(errno));
    _logger->push_log_error("PROC", "Coil Address", std::to_string(address));
    _logger->push_log_error("PROC", "read_len", std::to_string(read_len));
    _logger->push_log_error("PROC", "read_bits", std::to_string(read_bits));
    disconnect_modbus_tcp();
    return frb::to_int(frb::CommRes::Fail);
  }
  return read_bits;
}

int32_t frb::ModbusWrapper::write_data_tcp(int32_t address, int32_t status)
{
  // modbus 연결 되어있지 않으면 return
  if(!_is_connected) return frb::to_int(frb::CommRes::Fail);

  // data 쓰기
  std::lock_guard<std::mutex> lock(_mtx);
  size_t                      write_bits = modbus_write_bit(_modbus, address, status);

  // 쓰기 성공 여부 확인
  if(write_bits != 1)
  {
    _logger->push_log_error("PROC", "modbus_write_bits fail !!!", "");
    _logger->push_log_error("PROC", "Coil Address", std::to_string(address));
    _logger->push_log_error("PROC", "Status", std::to_string(status));
    disconnect_modbus_tcp();
    return frb::to_int(frb::CommRes::Fail);
  }
  return frb::to_int(frb::CommRes::Success);
}