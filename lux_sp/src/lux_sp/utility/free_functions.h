#pragma once

#include <iostream>
#include <string_view>
#include <variant>

#include <fmt/ostream.h>

namespace lux_sp::utility::free_functions {

template <typename T, typename... Args>
inline bool Is(const std::variant<Args...> &var) {
  return std::holds_alternative<T>(var);
}

template <typename T, typename... Args>
inline const T &Get(const std::variant<Args...> &var) {
  return std::get<T>(var);
}

template <typename T, typename... Args>
inline T &Get(std::variant<Args...> &var) {
  return std::get<T>(var);
}

inline void Fatal(std::string_view message) {
  fmt::print(std::cerr, "{}\n", message);
  exit(EXIT_FAILURE);
}

inline void Assert(bool condition, std::string_view message) {
  if (!condition) [[unlikely]] {
    Fatal(message);
  }
}

}  // namespace lux_sp::utility::free_functions
