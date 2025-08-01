#pragma once

#include <smart_cargo_libs/helper/enum_template.h>

namespace frb  // fieldro_bot
{

struct AcitonTarget
{
  int32_t  slave_id;
  int16_t  rpm;      // config 지정 rpm 저장 변수
  int16_t  acc;      // config 지정 acceleration 저장 변수
  int16_t  dec;      // config 지정 deceleration 저장 변수
  int16_t  ppr;      // pulse per revolution
  int32_t  p_limit;  // Software Positive Limit
  int32_t  n_limit;  // Software Negative Limit
  uint16_t homing_bit_0;
  uint16_t homing_bit_1;
  uint16_t homing_bit_2_to_7;
  uint16_t homing_bit_8;
};

enum class ModbusType
{
  None,

  Rs232,
  Rtu,
  Tcp,

  ModbusTypeEnd,
};

enum class CommRes
{
  None = 0,

  Success = 1,
  Fail    = -1,

  CommResEnd,
};

template <>
inline std::string to_string<frb::CommRes>(frb::CommRes value)
{
  switch(value)
  {
    case frb::CommRes::Success: return "Success";
    case frb::CommRes::Fail: return "Fail";

    default: return "Unknown";
  }
}
}  // namespace frb