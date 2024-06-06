#include <lux_sp/system_calls.h>

#include <utility>

namespace lux_sp {

int SystemCalls::pthread_setaffinity_np(
    pthread_t th, size_t cpusetsize,
    cpu_set_t *cpuset) noexcept(noexcept(::pthread_setaffinity_np(th,
                                                                  cpusetsize,
                                                                  cpuset))) {
  return ::pthread_setaffinity_np(std::move(th), std::move(cpusetsize),
                                  std::move(cpuset));
}

} // namespace lux_sp
