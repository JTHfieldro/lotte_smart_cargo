#include "rmd.h"

void frb::Rmd::push_recv_packet(const std::array<uint8_t, 8>& packet)
{
  std::lock_guard<std::mutex> lock(_lock);
  _recv_packet.push_back(packet);
}

void frb::Rmd::pop_recv_packet()
{
  if(_recv_packet.empty()) return;

  std::lock_guard<std::mutex> lock(_lock);
  std::array<uint8_t, 8>      packet = _recv_packet.front();
  _recv_packet.pop_front();

  frb::RmdDefine cmd = static_cast<frb::RmdDefine>(packet[0]);

  switch(cmd)
  {
    case frb::RmdDefine::MReadAbsPos: get_position(packet); break;
    case frb::RmdDefine::MotorReadStatus1: get_hw_status(packet); break;
    case frb::RmdDefine::MotorReadStatus2: get_current_speed(packet); break;
  }
}
