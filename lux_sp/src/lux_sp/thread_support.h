#pragma once

#include <sched.h>

#include <memory>

#include <lux_sp/system.h>

namespace lux_sp {

class ThreadSupport final {
 public:
  explicit ThreadSupport(std::unique_ptr<System> system) noexcept
      : system_{std::move(system)} {}
  ~ThreadSupport() = default;
  ThreadSupport(const ThreadSupport &) = delete;
  ThreadSupport(ThreadSupport &&) noexcept = default;
  ThreadSupport &operator=(const ThreadSupport &) = delete;
  ThreadSupport &operator=(ThreadSupport &&) noexcept = default;

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
