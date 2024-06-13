#pragma once

#include <cstdlib>
#include <iostream>
#include <string_view>

#include <fmt/ostream.h>

#include <lux_sp/exit.h>

namespace lux_sp {

class Predicates final {
 public:
  explicit Predicates(std::unique_ptr<Exit> exit) noexcept
      : exit_(std::move(exit)) {}
  ~Predicates() noexcept = default;
  Predicates(const Predicates &) = delete;
  Predicates(Predicates &&) noexcept = default;
  Predicates &operator=(const Predicates &) = delete;
  Predicates &operator=(Predicates &&) noexcept = default;

  void Assert(bool condition, std::string_view message) noexcept {
    if (!condition) [[unlikely]] {
      FatalError(message);
    }
  }

  void FatalError(std::string_view message) noexcept {
    fmt::print(std::cerr, "fatal error: {}\n", message);
    exit_->QuickExit(EXIT_FAILURE);
  }

 private:
  std::unique_ptr<Exit> exit_;
};

}  // namespace lux_sp
