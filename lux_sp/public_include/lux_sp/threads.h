#pragma once

#include <lux_sp/system_calls.h>

namespace lux_sp {

class Threads {
public:
  Threads() = delete;
  Threads(const Threads &) = delete;
  Threads(Threads &&) = delete;
  Threads &operator=(const Threads &) = delete;
  Threads &operator=(Threads &&) = delete;
  ~Threads() = delete;

  template <typename SystemCalls = SystemCalls>
  [[nodiscard]] static bool SetThreadAffinityToCore(int core_id) noexcept;
};

} // namespace lux_sp
