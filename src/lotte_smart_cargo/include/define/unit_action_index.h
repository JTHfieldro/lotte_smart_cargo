
#pragma once

#include <smart_cargo_libs/helper/enum_template.h>

namespace frb  // fieldro_bot
{

enum class UnitActionIndex
{
  None = 0x0000,  // None

  DoorOpen,
  DoorOpening,
  DoorClose,
  DoorClosing,
  DoorCancel,

  LiftFall,
  LiftFalling,
  LiftRise,
  LiftRising,

  ForkForward,
  ForkForwarding,
  ForkBackward,
  ForkBackwarding,

  RackForward,
  RackForwarding,
  RackBackward,
  RackBackwarding,

  MagnetForward,
  MagnetForwarding,
  MagnetBackward,
  MagnetBackwarding,
  MagnetReady,
  MagnetContact,

  TiltFall,
  TiltFalling,
  TiltRise,
  TiltRiseing,

  TargetSlot0,
  TargetSlot1,
  TargetSlot2,
  TargetSlot3,
  TargetSlot4,
  TargetSlot5,
  TargetSlot6,
  TargetSlot7,
  TargetSlot8,

  DiArmatureContactDetectionOn,
  DiArmatureContactDetectionOff,

  DoRatchUnlockFlagOn,
  DoRatchUnlockFlagOff,
  DoMagnetOnFlagOn,
  DoMagnetOnFlagOff,
  DoHandlerTiltRunFlagOn,
  DoHandlerTiltRunFlagOff,
  DoHandlerTiltDirectionFlagOn,
  DoHandlerTiltDirectionFlagOff,

  DockingLoadReq,
  DockingUnloadReq,
  DockingCancel,
  DockingStart,
  DockingLoadDockingStart,
  DockingUnloadDockingStart,
  DockingPermissionEnter,
  DockingLoadReqPkg,
  DockingUnloadReqPkg,
  DockingAttachSensor,
  DockingDetachSensor,
  Undocking,
  DockingDetectUndocking,
  DockingFinish,

  IsStart,     // main thread 시작 여부 확인
  Start,       // main thread 시작
  Init,        // 초기화
  InitFinish,  // 초기화 완료
  Homing,
  HomingFinish,
  Stop,
  AllUnitInitFinish,  // 모든 Unit 초기화 완료
  Finish,             // main thread 종료

  UnitActionIndexEnd,
};

template <>
inline std::string to_string<frb::UnitActionIndex>(frb::UnitActionIndex value)
{
  switch(value)
  {
    se frb::UnitActionIndex::None : return "None";

    case frb::UnitActionIndex::DoorOpen: return "DoorOpen";
    case frb::UnitActionIndex::DoorOpening: return "DoorOpening";
    case frb::UnitActionIndex::DoorClose: return "DoorClose";
    case frb::UnitActionIndex::DoorClosing: return "DoorClosing";
    case frb::UnitActionIndex::DoorCancel: return "DoorCancel";

    case frb::UnitActionIndex::LiftFall: return "LiftFall";
    case frb::UnitActionIndex::LiftFalling: return "LiftFalling";
    case frb::UnitActionIndex::LiftRise: return "LiftRise";
    case frb::UnitActionIndex::LiftRising: return "LiftRising";

    case frb::UnitActionIndex::ForkForward: return "ForkForward";
    case frb::UnitActionIndex::ForkForwarding: return "ForkForwarding";
    case frb::UnitActionIndex::ForkBackward: return "ForkBackward";
    case frb::UnitActionIndex::ForkBackwarding: return "ForkBackwarding";

    case frb::UnitActionIndex::RackForward: return "RackForward";
    case frb::UnitActionIndex::RackForwarding: return "RackForwarding";
    case frb::UnitActionIndex::RackBackward: return "RackBackward";
    case frb::UnitActionIndex::RackBackwarding: return "RackBackwarding";

    case frb::UnitActionIndex::MagnetForward: return "MagnetForward";
    case frb::UnitActionIndex::MagnetForwarding: return "MagnetForwarding";
    case frb::UnitActionIndex::MagnetBackward: return "MagnetBackward";
    case frb::UnitActionIndex::MagnetBackwarding: return "MagnetBackwarding";
    case frb::UnitActionIndex::MagnetReady: return "MagnetReady";
    case frb::UnitActionIndex::MagnetContact: return "MagnetContact";

    case frb::UnitActionIndex::TiltFall: return "TiltFall";
    case frb::UnitActionIndex::TiltFalling: return "TiltFalling";
    case frb::UnitActionIndex::TiltRise: return "TiltRise";
    case frb::UnitActionIndex::TiltRiseing: return "TiltRiseing";

    case frb::UnitActionIndex::TargetSlot0: return "TargetSlot0";
    case frb::UnitActionIndex::TargetSlot1: return "TargetSlot1";
    case frb::UnitActionIndex::TargetSlot2: return "TargetSlot2";
    case frb::UnitActionIndex::TargetSlot3: return "TargetSlot3";
    case frb::UnitActionIndex::TargetSlot4: return "TargetSlot4";
    case frb::UnitActionIndex::TargetSlot5: return "TargetSlot5";
    case frb::UnitActionIndex::TargetSlot6: return "TargetSlot6";
    case frb::UnitActionIndex::TargetSlot7: return "TargetSlot7";
    case frb::UnitActionIndex::TargetSlot8: return "TargetSlot8";

    case frb::UnitActionIndex::DoRatchUnlockFlagOn: return "DoRatchUnlockFlagOn";
    case frb::UnitActionIndex::DoRatchUnlockFlagOff: return "DoRatchUnlockFlagOff";
    case frb::UnitActionIndex::DoMagnetOnFlagOn: return "DoMagnetOnFlagOn";
    case frb::UnitActionIndex::DoMagnetOnFlagOff: return "DoMagnetOnFlagOff";
    case frb::UnitActionIndex::DoHandlerTiltRunFlagOn: return "DoHandlerTiltRunFlagOn";
    case frb::UnitActionIndex::DoHandlerTiltRunFlagOff: return "DoHandlerTiltRunFlagOff";
    case frb::UnitActionIndex::DoHandlerTiltDirectionFlagOn: return "DoHandlerTiltDirectionFlagOn";
    case frb::UnitActionIndex::DoHandlerTiltDirectionFlagOff: return "DoHandlerTiltDirectionFlagOff";

    case frb::UnitActionIndex::DockingStart: return "DockingStart";
    case frb::UnitActionIndex::DockingLoadReq: return "DockingLoadReq";
    case frb::UnitActionIndex::DockingUnloadReq: return "DockingUnloadReq";
    case frb::UnitActionIndex::DockingCancel: return "DockingCancel";
    case frb::UnitActionIndex::RequestEnter: return "RequestEnter";
    case frb::UnitActionIndex::DockingComplete: return "DockingComplete";
    case frb::UnitActionIndex::CompletedDetach: return "CompletedDetach";
    case frb::UnitActionIndex::PermitDocking: return "PermitDocking";
    case frb::UnitActionIndex::RequestDetach: return "RequestDetach";
    case frb::UnitActionIndex::RequestDoorClose: return "RequestDoorClose";

    case frb::UnitActionIndex::IsStart: return "IsStart";
    case frb::UnitActionIndex::Start: return "Start";
    case frb::UnitActionIndex::Init: return "Init";
    case frb::UnitActionIndex::InitFinish: return "InitFinish";
    case frb::UnitActionIndex::Homing: return "Homing";
    case frb::UnitActionIndex::HomingFinish: return "HomingFinish";
    case frb::UnitActionIndex::Stop: return "Stop";
    case frb::UnitActionIndex::AllUnitInitFinish: return "AllUnitInitFinish";
    case frb::UnitActionIndex::Finish: return "Finish";

    default: return "Unknown";
  }
}

}  // namespace frb