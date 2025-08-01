#pragma once

#include <smart_cargo_libs/helper/global_helper.h>
#include <smart_cargo_libs/helper/logger.h>
#include <smart_cargo_libs/helper/thread_action_info.h>

#include "rmd_define.h"

namespace frb  // fieldro_bot
{
class Rmd
{
private:
  Rmd() {}

public:
  Rmd(const std::string& config_path, frb::Logger* logger, std::function<void(std::array<uint8_t, 8>)> send_packet_callback);
  ~Rmd();

public:
  void control(const std::string& str);  ///< User Command를 받아서 처리하는 함수

  void control_speed(const int32_t rpm);
  void control_abs_position(const int32_t target_position);
  void control_inc_position(const int32_t target_position);
  void control_stop();

  void push_recv_packet(const std::array<uint8_t, 8>& packet);  ///< 수신된 패킷을 _recv_packet에 저장

private:
  frb::Logger*           _logger;
  frb::ThreadActionInfo* _thread_info;
  std::string            _config_path;
  std::mutex             _lock;

  std::list<std::array<uint8_t, 8>>           _recv_packet;           ///< 수신된 패킷을 저장하는 리스트
  std::function<void(std::array<uint8_t, 8>)> _send_packet_callback;  ///< 송신할 패킷을 처리하는 콜백 함수

  int16_t        _can_send_id;
  int16_t        _can_recv_id;
  frb::RmdDefine _control_mode;

  int32_t _rpm_ctrl_speed;     ///< frb::RmdDefine::CCtrlSpeed RPM 저장 변수
  int32_t _rpm_ctrl_position;  ///< frb::RmdDefine::CCtrlPosition, CCtrlIncPosition  RPM 저장 변수
  double  _position_max;
  double  _position_min;
  double  _position_gap;
  double  _position_home;
  double  _position_current;

private:
  void update();       ///< 실제 Thread 동작 함수
  void initialize();   ///< 초기화 함수
  void load_option();  ///< Config 설정 파일 로드 함수

  void control_shutdown();
  void control_brake(const bool flag);
  void control_reset();

  void pop_recv_packet();

  //* RMD Packet Setter
  std::array<uint8_t, 8> set_packet_cmd(const frb::RmdDefine cmd);            ///< command 발신용 패킷 생성
  std::array<uint8_t, 8> set_packet_speed(const int32_t rpm = 0);             ///< speed control 발신용 패킷 생성
  std::array<uint8_t, 8> set_packet_position(const int32_t pos);              ///< absolute position control 발신용 패킷 생성
  std::array<uint8_t, 8> set_packet_inc_position(const int32_t position);     ///< incremental position control 발신용 패킷 생성
  std::array<uint8_t, 8> set_packet_inc_position_angle(const int32_t angle);  ///< incremental position control 발신용 패킷 생성(0~360 각도 제어)

  void set_home();
  void set_position();

  void get_position();

  int32_t recv_position(const std::array<uint8_t, 8>& packet);
  bool    recv_hw_status(const std::array<uint8_t, 8>& packet);
  int32_t recv_current_speed(const std::array<uint8_t, 8>& packet);
};
}  // namespace frb