#pragma once

#include <smart_cargo_libs/helper/enum_template.h>

namespace frb  // fieldro_bot
{

// 접두어
// M : MultiTurn
// C : Closed Loop
// S : SingleTurn
// CTRL : Control
enum class RmdDefine
{
  NextPacketNone = 0x00,  // 다음에 전송할 패킷이 없음

  MReadCurrentPos        = 0x60,  // 멀티턴 	: Encoder 위치값 읽기
  MReadHomePos           = 0x61,  // 멀티턴 	: Home Position 위치값 읽기
  MReadOffsetPos         = 0x62,  // 멀티턴 	: Offset 위치값 읽기
  MWriteOffsetPos        = 0x63,  // 멀티턴 	: Offset 값 설정
  MWriteOffsetPosCurrent = 0x64,  // 멀티턴 	:
                                  // Offset 값 설정 (현재 위치를 Offset 값으로 설정)
  MReadAbsPos  = 0x92,            // 멀티턴		: 현재 각도 읽기
  SReadEncoder = 0x90,            // 싱글턴		:
                                  //      각 엔코더 값 요청 (current_pos, orignal_pos, zero_pos)
  SReadAbsPos      = 0x94,        // 싱글턴 	  :	현재 각도 읽기 (0 ~ 359.99)
  SCtrlPosition    = 0xA6,        // 싱글턴	  : Position Control
  CCtrlTorque      = 0xA1,        // 폐쇄루프  : Torque Control
  CCtrlSpeed       = 0xA2,        // 폐쇄루프  : Speed control
  CCtrlPosition    = 0xA4,        // 폐쇄루프  : Position Control (위치 == 각도)
  CCtrlIncPosition = 0xA8,        // 폐쇄루프	:
                                  // Position Control (현재 위치를 기준으로 증분 제어)
  MotorReadPid = 0x30,            // 모터		:
                                  // 현재 KI, KP 등의 인자값 읽기 요청
  MotorWritePidRam = 0x31,        // 모터		: KI, KP 등의 인자값을 Motor에
                                  // 쓰기  (Motor PowerOff 되면 사라짐)
  MotorWritePidRom = 0x32,        // 모터		: KI, KP 등의 인자값을 Motor에
                                  // 쓰기  (Motor PowerOff 되어도 사라지지 않음)
  MotorReadAccel      = 0x42,     // 모터		: 가속도 값 읽기
  MotorWriteAccel     = 0x43,     // 모터  	: 가속도 값 쓰기
  MotorReadMode       = 0x70,     // 모터 		: 동작 모드 읽기
  MotorReadPower      = 0x71,     // 모터 		: 현재 파워 읽기
  MotorCtrlReset      = 0x76,     // 모터		: 리셋
  MotorCtrlBreakOff   = 0x77,     // 모터 		: 브레이크 해제
  MotorCtrlBreakOn    = 0x78,     // 모터 		: 브레이크 ON
  MotorCtrlId         = 0x79,     // 모터 		: ID 읽기 또는 설정
  MotorCtrlShutdown   = 0x80,     // 모터 		: 끄기
  MotorCtrlStop       = 0x81,     // 모터 		: 멈춤
  MotorReadStatus1    = 0x9A,     // 모터		: 온도, BreakCmd, Voltage, Error
  MotorReadStatus2    = 0x9C,     // 모터 		: 온도, Torque, Output Speed, Output Angle
  MotorReadStatus3    = 0x9D,     // 모터	  : 온도, A, B, C 상 전류 데이터
  MotorReadRuntime    = 0xB1,     // 모터	  : 가동 시간
  MotorReadUpdatedate = 0xB2,     // 모터 		: 버전 업데이트 날짜
  MotorWriteTimeout   = 0xB3,     // 모터 		:
                                  // 통신 TimeOut 설정 (브레이크 잠금이 차단됨)
  MotorWriteBaudrate = 0xB4,      // 모터 		: 통신 속도 설정
};

template <>
inline std::string to_string<frb::RmdDefine>(frb::RmdDefine value)
{
  switch(value)
  {
    case RmdDefine::MReadCurrentPos: return "MReadCurrentPos";

    case RmdDefine::MReadHomePos: return "MReadHomePos";
    case RmdDefine::MReadOffsetPos: return "MReadOffsetPos";
    case RmdDefine::MWriteOffsetPos: return "MWriteOffsetPos";
    case RmdDefine::MWriteOffsetPosCurrent: return "MWriteOffsetPosCurrent";
    case RmdDefine::MReadAbsPos: return "MReadAbsPos";
    case RmdDefine::SReadEncoder: return "SReadEncoder";
    case RmdDefine::SReadAbsPos: return "SReadAbsPos";
    case RmdDefine::SCtrlPosition: return "SCtrlPosition";
    case RmdDefine::CCtrlTorque: return "CCtrlTorque";
    case RmdDefine::CCtrlSpeed: return "CCtrlSpeed";
    case RmdDefine::CCtrlPosition: return "CCtrlPosition";
    case RmdDefine::CCtrlIncPosition: return "CCtrlIncPosition";
    case RmdDefine::MotorReadPid: return "MotorReadPid";
    case RmdDefine::MotorWritePidRam: return "MotorWritePidRam";
    case RmdDefine::MotorWritePidRom: return "MotorWritePidRom";
    case RmdDefine::MotorReadAccel: return "MotorReadAccel";
    case RmdDefine::MotorWriteAccel: return "MotorWriteAccel";
    case RmdDefine::MotorReadMode: return "MotorReadMode";
    case RmdDefine::MotorReadPower: return "MotorReadPower";
    case RmdDefine::MotorCtrlReset: return "MotorCtrlReset";
    case RmdDefine::MotorCtrlBreakOff: return "MotorCtrlBreakOff";
    case RmdDefine::MotorCtrlBreakOn: return "MotorCtrlBreakOn";
    case RmdDefine::MotorCtrlId: return "MotorCtrlId";
    case RmdDefine::MotorCtrlShutdown: return "MotorCtrlShutdown";
    case RmdDefine::MotorCtrlStop: return "MotorCtrlStop";
    case RmdDefine::MotorReadStatus1: return "MotorReadStatus1";
    case RmdDefine::MotorReadStatus2: return "MotorReadStatus2";
    case RmdDefine::MotorReadStatus3: return "MotorReadStatus3";
    case RmdDefine::MotorReadRuntime: return "MotorReadRuntime";
    case RmdDefine::MotorReadUpdatedate: return "MotorReadUpdatedate";
    case RmdDefine::MotorWriteTimeout: return "MotorWriteTimeout";
    case RmdDefine::MotorWriteBaudrate: return "MotorWriteBaudrate";

    default: return "Unknown";
  }
}
}  // namespace frb