#pragma once

#include <sched.h>

#include <memory>

#include <lux_sp/system.h>

namespace lux_sp {

class ConcurrencySupport final {
 public:
  explicit ConcurrencySupport(std::unique_ptr<System> system) noexcept
      : system_{std::move(system)} {}
  ~ConcurrencySupport() = default;
  ConcurrencySupport(const ConcurrencySupport &) = delete;
  ConcurrencySupport(ConcurrencySupport &&) noexcept = default;
  ConcurrencySupport &operator=(const ConcurrencySupport &) = delete;
  ConcurrencySupport &operator=(ConcurrencySupport &&) noexcept = default;

  [[nodiscard]] bool SetThreadAffinityToCore(int core_id) const noexcept {
    auto cpu_set = cpu_set_t{};
    CPU_ZERO(&cpu_set);
    CPU_SET(core_id, &cpu_set);
    return system_->pthread_setaffinity_np(::pthread_self(), sizeof(cpu_set_t),
                                           &cpu_set) == 0;
  }

 private:
  std::unique_ptr<System> system_;
};

}  // namespace lux_sp
