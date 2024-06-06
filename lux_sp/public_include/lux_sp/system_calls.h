#pragma once

#include <pthread.h>

namespace lux_sp {

class SystemCalls {
public:
  SystemCalls() = delete;
  ~SystemCalls() = delete;
  SystemCalls(const SystemCalls &) = delete;
  SystemCalls(SystemCalls &&) = delete;
  SystemCalls &operator=(const SystemCalls &) = delete;
  SystemCalls &operator=(SystemCalls &&) = delete;

  [[nodiscard]] static int pthread_setaffinity_np(
      pthread_t th, size_t cpusetsize,
      cpu_set_t *cpuset) noexcept(noexcept(::pthread_setaffinity_np(th,
                                                                    cpusetsize,
                                                                    cpuset)));
};

} // namespace lux_sp
