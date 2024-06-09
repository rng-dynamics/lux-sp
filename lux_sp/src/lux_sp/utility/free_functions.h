#pragma once

#include <variant>
namespace lux_sp::utility {

namespace free_functions {

template <typename T, typename... Args>
bool Is(const std::variant<Args...> &var) {
  return std::holds_alternative<T>(var);
}

template <typename T, typename... Args>
const T &Get(const std::variant<Args...> &var) {
  return std::get<T>(var);
}

template <typename T, typename... Args>
T &Get(std::variant<Args...> &var) {
  return std::get<T>(var);
}

}  // namespace free_functions

}  // namespace lux_sp::utility
