#pragma once

#include <iostream>
#include <string_view>

#include <fmt/ostream.h>

#include <lux_sp/runtime.h>

namespace lux_sp {

class Invariants final {
 public:
  explicit Invariants(std::unique_ptr<Runtime> runtime)
      : runtime_{std::move(runtime)} {}
  Invariants() = delete;
  ~Invariants() noexcept = default;
  Invariants(const Invariants &) = delete;
  Invariants(Invariants &&) noexcept = default;
  Invariants &operator=(const Invariants &) = delete;
  Invariants &operator=(Invariants &&) noexcept = default;

  void Fatal(std::string_view message) noexcept {
    fmt::print(std::cerr, "{}\n", message);
    runtime_->terminate();
  }

  void Assert(bool condition, std::string_view message) noexcept {
    if (!condition) [[unlikely]] {
      Fatal(message);
    }
  }

 private:
  std::unique_ptr<Runtime> runtime_;
};

}  // namespace lux_sp
