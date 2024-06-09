#pragma once

#include <fmt/core.h>
#include <fmt/ostream.h>

#include <cstdlib>
#include <iostream>

namespace lux_sp {

class Utility final {
 public:
  Utility() = delete;
  Utility(const Utility &) = delete;
  Utility(Utility &&) = delete;
  Utility &operator=(const Utility &) = delete;
  Utility &operator=(Utility &&) = delete;

  inline static void Assert(bool condition, std::string_view message) {
    if (!condition) [[unlikely]] {
      Fatal(std::move(message));
    }
  }

  inline static void Fatal(std::string_view message) {
    fmt::print(std::cerr, "{}\n", message);
    exit(EXIT_FAILURE);
  }
};
}  // namespace lux_sp
