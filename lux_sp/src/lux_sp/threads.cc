#include <pthread.h>

#include <lux_sp/threads.h>

namespace lux_sp {

bool Threads::SetThreadAffinityToCore(int core_id) noexcept {
  auto cpu_set = cpu_set_t{};
  CPU_ZERO(&cpu_set);
  CPU_SET(core_id, &cpu_set);
  return ::pthread_setaffinity_np(::pthread_self(), sizeof(cpu_set_t),
                                             &cpu_set) == 0;
}

} // namespace lux_sp
