#pragma once

#include <cstdlib>
#include <iostream>
#include <string_view>

#include <fmt/ostream.h>

#include <lux_sp/no_return.h>

namespace lux_sp {

class Assertions final {
 public:
  explicit Assertions(std::unique_ptr<NoReturn> exit) noexcept
      : no_return_(std::move(exit)) {}
  ~Assertions() noexcept = default;
  Assertions(const Assertions &) = delete;
  Assertions(Assertions &&) noexcept = default;
  Assertions &operator=(const Assertions &) = delete;
  Assertions &operator=(Assertions &&) noexcept = default;

  void Assert(bool condition, std::string_view message) noexcept {
    if (!condition) [[unlikely]] {
      FatalError(message);
    }
  }

  void FatalError(std::string_view message) noexcept {
    fmt::print(std::cerr, "fatal error: {}\n", message);
    no_return_->QuickExit(EXIT_FAILURE);
  }

 private:
  std::unique_ptr<NoReturn> no_return_;
};

}  // namespace lux_sp
