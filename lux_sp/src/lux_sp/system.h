#pragma once

#include <pthread.h>

#include <cstddef>

namespace lux_sp {

class System {
 public:
  virtual ~System() = default;

  virtual int pthread_setaffinity_np(pthread_t thread, std::size_t cpusetsize,
                                     cpu_set_t *cpuset) const noexcept = 0;
};

}  // namespace lux_sp
