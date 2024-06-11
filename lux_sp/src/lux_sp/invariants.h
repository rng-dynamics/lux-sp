#pragma once

#include <iostream>
#include <string_view>

#include <fmt/ostream.h>

namespace lux_sp {

class Invariants final {
 public:
  Invariants() noexcept = default;
  ~Invariants() noexcept = default;
  Invariants(const Invariants &) = default;
  Invariants(Invariants &&) noexcept = default;
  Invariants &operator=(const Invariants &) = default;
  Invariants &operator=(Invariants &&) noexcept = default;

  static void FatalError(std::string_view message) noexcept {
    fmt::print(std::cerr, "fatal error: {}\n", message);
    std::terminate();
  }

  static void Assert(bool condition, std::string_view message) noexcept {
    if (!condition) [[unlikely]] {
      FatalError(message);
    }
  }
};

}  // namespace lux_sp
