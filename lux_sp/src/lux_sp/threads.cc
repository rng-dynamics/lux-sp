#include <lux_sp/threads.h>

namespace lux_sp {

template <typename SystemCalls>
bool Threads::SetThreadAffinityToCore(int core_id) noexcept {
  auto cpu_set = cpu_set_t{};
  CPU_ZERO(&cpu_set);
  CPU_SET(core_id, &cpu_set);
  return SystemCalls::pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),
                                             &cpu_set) == 0;
}

template bool Threads::SetThreadAffinityToCore<SystemCalls>(int core_id) noexcept;

} // namespace lux_sp
