#pragma once

#include <pthread.h>

#include <cstddef>

#include <lux_sp/system.h>

namespace lux_sp {

class ConcreteSystem final : public System {
 public:
  [[nodiscard]] int pthread_setaffinity_np(
      pthread_t thread, std::size_t cpusetsize,
      cpu_set_t *cpuset) const noexcept override {
    return ::pthread_setaffinity_np(thread, cpusetsize, cpuset);
  }
};

}  // namespace lux_sp
