#pragma once

#include <smart_cargo_libs/helper/enum_template.h>

namespace frb  // fieldro_bot
{
enum class DiSignal
{
  None = 0,  // 미사용

  DiRack00 = 1,   // NO, bottom slot is num_00
  DiRack01 = 2,   // NO
  DiRack02 = 3,   // NO
  DiRack03 = 4,   // NO
  DiRack04 = 5,   // NO
  DiRack05 = 6,   // NO
  DiRack06 = 7,   // NO
  DiRack07 = 8,   // NO
  DiRack08 = 9,   // NO
  DiRack09 = 10,  // NO

  DiDoorClose = 11,  // NC, 적재함 Door 폐쇄 감지
  DiDoorOpen  = 12,  // NC, 적재함 Door 개방 감지

  Signal13 = 13,
  Signal14 = 14,
  Signal15 = 15,
  Signal16 = 16,

  DiMagnetUnitOrigin = 17,  // NC, Magnet Unit Origin(Handler 중앙)
  DiMagnetUnitFront  = 18,  // NC, Magnet Unit Front(AGV 외측 방향)
  DiMagnetUnitRear   = 19,  // NC, Magnet Unit Rear(AGV 내측 방향)

  DiArmatureContactDetection = 20,  // NC, Armature Contact Detection
  DiTrayContactDetection     = 21,  // NO, Tray Contact Detection

  DiHandlerTiltMinLimit = 22,  // NO, Tilt Down Limit
  DiHandlerTiltMaxLimit = 23,  // NO, Tilt Up Limit(하역)

  DiRackForkBackwardLimit = 24,  // NC, Rack Backward Status(AGV 내측 방향)
  DiRackForkForwardLimit  = 25,  // NC, Rack Forward Status(AGV 외측 방향)

  DiRatchLockFlag = 26,  // NO, Ratch lock Flag

  DiCount = 26,  // DI_SIGANL 개수
  DiSignalEnd,
};

template <>
inline std::string to_string<frb::DiSignal>(frb::DiSignal value)
{
  switch(value)
  {
    case frb::DiSignal::None: return "None";

    case frb::DiSignal::DiRack00: return "DiRack00";
    case frb::DiSignal::DiRack01: return "DiRack01";
    case frb::DiSignal::DiRack02: return "DiRack02";
    case frb::DiSignal::DiRack03: return "DiRack03";
    case frb::DiSignal::DiRack04: return "DiRack04";
    case frb::DiSignal::DiRack05: return "DiRack05";
    case frb::DiSignal::DiRack06: return "DiRack06";
    case frb::DiSignal::DiRack07: return "DiRack07";
    case frb::DiSignal::DiRack08: return "DiRack08";
    case frb::DiSignal::DiRack09: return "DiRack09";

    case frb::DiSignal::DiDoorClose: return "DiDoorClose";
    case frb::DiSignal::DiDoorOpen: return "DiDoorOpen";

    case frb::DiSignal::Signal13: return "Signal13";
    case frb::DiSignal::Signal14: return "Signal14";
    case frb::DiSignal::Signal15: return "Signal15";
    case frb::DiSignal::Signal16: return "Signal16";

    case frb::DiSignal::DiMagnetUnitOrigin: return "DiMagnetUnitOrigin";
    case frb::DiSignal::DiMagnetUnitFront: return "DiMagnetUnitFront";
    case frb::DiSignal::DiMagnetUnitRear: return "DiMagnetUnitRear";

    case frb::DiSignal::DiArmatureContactDetection: return "DiArmatureContactDetection";
    case frb::DiSignal::DiTrayContactDetection: return "DiTrayContactDetection";

    case frb::DiSignal::DiHandlerTiltMinLimit: return "DiHandlerTiltMinLimit";
    case frb::DiSignal::DiHandlerTiltMaxLimit: return "DiHandlerTiltMaxLimit";

    case frb::DiSignal::DiRackForkBackwardLimit: return "DiRackForkBackwardLimit";
    case frb::DiSignal::DiRackForkForwardLimit: return "DiRackForkForwardLimit";

    case frb::DiSignal::DiRatchLockFlag: return "DiRatchLockFlag";

    default: return "Unknown";
  }
}

/**
/* @brief   wago output signal define
/* @note    숫자 = 511 + wago의 indicator LED 번호
*/
enum class DoSignal
{
  DoRatchUnlockFlag = 512,  // Ratch Unlock Flag

  DoMagnetOnFlag = 513,  // Magnet On Flag

  DoHandlerTiltRunFlag       = 514,  // Handler Tilt Motor On(run) Flag
  DoHandlerTiltDirectionFlag = 515,  // Handler Tilt Motor Direction Flag
                                     // On(CW, Flat) Off(CCW, Right)

  DoCount = 4,  // DO_SIGNAL 개수
  DoSignalEnd,
};

template <>
inline std::string to_string<frb::DoSignal>(frb::DoSignal value)
{
  switch(value)
  {
    case DoSignal::DoRatchUnlockFlag: return "DoRatchUnlockFlag";

    case DoSignal::DoMagnetOnFlag: return "DoMagnetOnFlag";

    case DoSignal::DoHandlerTiltRunFlag: return "DoHandlerTiltRunFlag";
    case DoSignal::DoHandlerTiltDirectionFlag: return "DoHandlerTiltDirectionFlag";

    default: return "Unknown";
  }
}

enum class SignalValue
{
  SignalOff = 0,  // Signal Off
  SignalOn  = 1,  // Signal On
  SignalValueEnd
};

template <>
inline std::string to_string<frb::SignalValue>(frb::SignalValue value)
{
  switch(value)
  {
    case frb::SignalValue::SignalOff: return "SignalOff";
    case frb::SignalValue::SignalOn: return "SignalOn";
    default: return "Unknown";
  }
}

inline std::string no_to_string(int32_t value)
{
  switch(static_cast<frb::SignalValue>(value))
  {
    case frb::SignalValue::SignalOff: return "SignalOff";
    case frb::SignalValue::SignalOn: return "SignalOn";
    default: return "Unknown";
  }
}

inline std::string nc_to_string(int32_t value)
{
  switch(static_cast<frb::SignalValue>(value))
  {
    case frb::SignalValue::SignalOff: return "SignalOn";
    case frb::SignalValue::SignalOn: return "SignalOff";
    default: return "Unknown";
  }
}

}  // namespace frb