#include "rmd.h"

/**
 * @brief			command 발신용 패킷 생성
 * @param[in]
 * @return
 * @note
 */
std::array<uint8_t, 8> frb::Rmd::set_packet_cmd(const frb::RmdDefine cmd)
{
  return {static_cast<uint8_t>(cmd), 0, 0, 0, 0, 0, 0, 0};
}

/**
 * @brief     speed control 발신용 패킷 생성
 * @param[in]
 * @return
 * @note 			Closed_Loop Control
 * @note 			0.01[dps/LSB]
 * @note 			60[rpm] = 36000[dps/LSB]
 */
std::array<uint8_t, 8> frb::Rmd::set_packet_speed(int32_t rpm = 0)
{
  return {static_cast<uint8_t>(frb::RmdDefine::CCtrlSpeed), 0, 0, 0,
          rpm & 0xFF,           // speedControl
          (rpm >> 8) & 0xFF,    // speedControl >> 8
          (rpm >> 16) & 0xFF,   // speedControl >> 16
          (rpm >> 24) & 0xFF};  // speedControl >> 24
}

/**
 * @brief     absolute position control 발신용 패킷 생성
 * @param[in]
 * @return
 * @note 			Position Control
 * @note 			0.01[degree/LSB]
 * @note 			36000 = 360[degree]
 * @note 			1[dps/LSB]
 */
std::array<uint8_t, 8> frb::Rmd::set_packet_position(const int32_t position)
{
  return {static_cast<uint8_t>(frb::RmdDefine::CCtrlPosition), 0,
          _rpm_ctrl_position & 0xFF,         // maxSpeed
          (_rpm_ctrl_position >> 8) & 0xFF,  // maxSpeed >> 8
          position & 0xFF,                   // angleControl
          (position >> 8) & 0xFF,            // angleControl >> 8
          (position >> 16) & 0xFF,           // angleControl >> 16
          (position >> 24) & 0xFF}           // angleControl >> 24
}

/**
 * @brief     incremental position control 발신용 패킷 생성(0~360 각도 제어)
 * @param[in]
 * @return
 * @note 			Position Control
 * @note 			0.01[degree/LSB]
 * @note 			36000 = 360[degree]
 * @note 			1[dps/LSB]
 */
std::array<uint8_t, 8> frb::Rmd::set_packet_inc_position(const int32_t position)
{
  return {static_cast<uint8_t>(frb::RmdDefine::CCtrlIncPosition), 0,
          _rpm_ctrl_position & 0xFF,         // maxSpeed
          (_rpm_ctrl_position >> 8) & 0xFF,  // maxSpeed >> 8
          position & 0xFF,                   // angleControl
          (position >> 8) & 0xFF,            // angleControl >> 8
          (position >> 16) & 0xFF,           // angleControl >> 16
          (position >> 24) & 0xFF}           // angleControl >> 24
}

std::array<uint8_t, 8> frb::Rmd::set_packet_inc_position_angle(const int32_t angle)
{
  return {static_cast<uint8_t>(frb::RmdDefine::CCtrlIncPosition), 0,
          _rpm_ctrl_position & 0xFF,         // maxSpeed
          (_rpm_ctrl_position >> 8) & 0xFF,  // maxSpeed >> 8
          (angle * 100) & 0xFF,              // angleControl
          ((angle * 100) >> 8) & 0xFF,       // angleControl >> 8
          ((angle * 100) >> 16) & 0xFF,      // angleControl >> 16
          ((angle * 100) >> 24) & 0xFF}      // angleControl >> 24
}