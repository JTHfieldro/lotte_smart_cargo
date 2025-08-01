#include "modbus_wrapper.h"

int32_t frb::ModbusWrapper::read_data_rtu(int32_t id, int32_t address, int32_t read_len,
                                          uint16_t* dest)
{
  std::lock_guard<std::mutex> lock(_mtx);
  set_slave_id_rtu(id);
  // modbus 연결 되어있지 않으면 return
  if(!_is_connected) return frb::to_int(frb::CommRes::Fail);

  // data 읽기
  int32_t read_bits = modbus_read_registers(_modbus, address, read_len, dest);

  // 읽어들인 Data 길이 확인
  if(read_bits != read_len)
  {
    _logger->push_log_error("PROC", "modbus_read_registers fail !!!", "");
    _logger->push_log_error("PROC", "Slave ID", std::to_string(_slave_id));
    _logger->push_log_error("PROC", "Error Number", modbus_strerror(errno));
    _logger->push_log_error("PROC", "Register Address", std::to_string(address));
    _logger->push_log_error("PROC", "read_len", std::to_string(read_len));
    _logger->push_log_error("PROC", "read_registers", std::to_string(read_bits));
    disconnect_modbus_rtu();
    return frb::to_int(frb::CommRes::Fail);
  }
  return read_bits;
}

int32_t frb::ModbusWrapper::write_data_rtu(int32_t id, int32_t address, int32_t status)
{
  std::lock_guard<std::mutex> lock(_mtx);
  set_slave_id_rtu(id);
  // modbus 연결 되어있지 않으면 return
  if(!_is_connected) return frb::to_int(frb::CommRes::Fail);

  // data 쓰기
  int32_t write_bits = modbus_write_register(_modbus, address, status);

  // 쓰기 성공 여부 확인
  if(write_bits != 1)
  {
    _logger->push_log_error("PROC", "modbus_write_register fail !!!", "");
    _logger->push_log_error("PROC", "Slave ID", std::to_string(_slave_id));
    _logger->push_log_error("PROC", "Error Number", modbus_strerror(errno));
    _logger->push_log_error("PROC", "Register Address", std::to_string(address));
    _logger->push_log_error("PROC", "Status", std::to_string(status));
    disconnect_modbus_rtu();
    return frb::to_int(frb::CommRes::Fail);
  }
  return write_bits;
}

int32_t frb::ModbusWrapper::write_datas_rtu(int32_t id, int32_t address, int32_t size,
                                            const uint16_t* data)
{
  std::lock_guard<std::mutex> lock(_mtx);
  set_slave_id_rtu(id);
  // modbus 연결 되어있지 않으면 return
  if(!_is_connected) return frb::to_int(frb::CommRes::Fail);

  // data 쓰기
  int32_t write_bits = modbus_write_registers(_modbus, address, size, data);

  // 쓰기 성공 여부 확인
  if(write_bits != size)
  {
    _logger->push_log_error("PROC", "modbus_write_registers fail !!!", "");
    _logger->push_log_error("PROC", "Slave ID", std::to_string(_slave_id));
    _logger->push_log_error("PROC", "Error Number", modbus_strerror(errno));
    _logger->push_log_error("PROC", "Register Address", std::to_string(address));
    disconnect_modbus_rtu();
    return frb::to_int(frb::CommRes::Fail);
  }
  return write_bits;
}