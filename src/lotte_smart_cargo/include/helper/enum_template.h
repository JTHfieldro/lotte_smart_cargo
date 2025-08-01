
#pragma once

#include <cstdint>
#include <string>
#include <type_traits>

namespace frb  // fieldro_bot
{
/**
 * @brief      enum => int로 변환
 * @param[in]  enum class EnumType : enum type(name)
 * @return     int32_t : enum type을 int로 변환한 값
 * @note       to_int(EnumType value) 형태로 사용
 */
template <typename EnumType>
constexpr int32_t to_int(EnumType value)
{
  static_assert(std::is_enum<EnumType>::value, "Template parameter must be an enum type");
  return static_cast<int32_t>(value);
}

/**
 * @brief      int => enum 변환
 * @param[in]  int32_t value : int value
 * @return     enum class EnumType : int를 enum type으로 변환한 값
 * @note       to_enum<EnumType>(int32_t value) 형태로 사용
 */
template <typename EnumType, typename T,
          typename = typename std::enable_if<std::is_same<T, int32_t>::value>::type>
constexpr EnumType to_enum(T value)
{
  static_assert(std::is_enum<EnumType>::value, "EnumType must be an enum");
  return static_cast<EnumType>(value);
}

/**
 * @brief      enum => string으로 변환
 * @param[in]  enum class EnumType : enum type(name)
 * @return     std::string : enum type중 value에 해당하는 string(name)
 * @note       to_string(EnumType value) 형태로 사용
 * @attention  각 enum class가 정의된 header에 구현 되어야 한다.
 */
template <typename EnumType>
std::string to_string(EnumType value);

/**
 * @brief      int => string 변환
 * @param[in]  int32_t value : int value
 * @return     std::string : int를 enum class 변수와 매칭되는 string으로 변환한 값
 * @note       to_string<EnumType, int32_t>(int32_t value) 형태로 사용
 * @attention  각 enum class가 정의된 header에서 구현 되어야 한다.
 */
template <typename EnumType, typename T,
          typename = typename std::enable_if<std::is_same<T, int32_t>::value>::type>
std::string to_string(T value);

/**
 * @brief      string => enum 변환
 * @param[in]  std::string : string value
 * @return     enum class EnumType : string을 enum type으로 변환한 값
 * @note       to_enum<EnumType, std::string>(std::string value) 형태로 사용
 * @attention  각 enum class가 정의된 header에서 구현 되어야 한다.
 */
template <typename EnumType, typename T,
          typename = typename std::enable_if<std::is_same<T, std::string>::value>::type>
EnumType to_enum(const T& str);

/**
 * @brief      string => int 변환
 * @param[in]  std::string : string value
 * @return     enum class EnumType : string 매칭되는 값을 int로 변환한 값
 * @note       to_string<EnumType, std::string>(std::string value) 형태로 사용
 * @attention  각 enum class가 정의된 header에서 구현 되어야 한다.
 */
template <typename EnumType, typename T,
          typename = typename std::enable_if<std::is_same<T, std::string>::value>::type>
int32_t to_int(const T& str);
}  // namespace frb