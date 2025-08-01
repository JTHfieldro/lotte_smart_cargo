#pragma once

#include <smart_cargo_libs/helper/enum_template.h>

namespace frb  // fieldro_bot
{

enum class UnitIndex
{
  // Absolute
  None = -1,
  All  = 0x00,  // 모든 유닛

  // Smart Cargo
  SmartCargo = 0x01,
  Door,
  Rack,
  Magnet,
  Leadshine,
  Fork,
  Lift,
  Pio,
  Io,
  Tilt,

  CargoInit,
  CargoDrop,
  CargoDocking,

  UnitIndexEnd,
};

template <>
inline std::string to_string<frb::UnitIndex>(frb::UnitIndex value)
{
  switch(value)
  {
    case frb::UnitIndex::None: return "None";
    case frb::UnitIndex::All: return "All";

    case frb::UnitIndex::SmartCargo: return "SmartCargo";
    case frb::UnitIndex::Door: return "Door";
    case frb::UnitIndex::Rack: return "Rack";
    case frb::UnitIndex::Magnet: return "Magnet";
    case frb::UnitIndex::Leadshine: return "Leadshine";
    case frb::UnitIndex::Fork: return "Fork";
    case frb::UnitIndex::Lift: return "Lift";
    case frb::UnitIndex::Pio: return "Pio";
    case frb::UnitIndex::Io: return "Io";
    case frb::UnitIndex::Tilt: return "Tilt";

    default: return "Unknown";
  }
}

}  // namespace frb