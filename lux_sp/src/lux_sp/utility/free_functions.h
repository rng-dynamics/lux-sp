#pragma once

#include <variant>

#include <fmt/ostream.h>

namespace lux_sp::utility::free_functions {

template <typename T, typename... Args>
inline bool Is(const std::variant<Args...> &var) noexcept {
  return std::holds_alternative<T>(var);
}

template <typename T, typename... Args>
inline const T &Get(const std::variant<Args...> &var) noexcept {
  return std::get<T>(var);
}

template <typename T, typename... Args>
inline T &Get(std::variant<Args...> &var) {
  return std::get<T>(var);
}

}  // namespace lux_sp::utility::free_functions
