#pragma once

#include <iostream>
#include <string_view>

#include <fmt/ostream.h>

namespace lux_sp {

class Predicates final {
 public:
  Predicates() noexcept = default;
  ~Predicates() noexcept = default;
  Predicates(const Predicates &) = default;
  Predicates(Predicates &&) noexcept = default;
  Predicates &operator=(const Predicates &) = default;
  Predicates &operator=(Predicates &&) noexcept = default;

  static void Assert(bool condition, std::string_view message) noexcept {
    if (!condition) [[unlikely]] {
      FatalError(message);
    }
  }

  static void FatalError(std::string_view message) noexcept {
    fmt::print(std::cerr, "fatal error: {}\n", message);
    std::terminate();
  }
};

}  // namespace lux_sp
