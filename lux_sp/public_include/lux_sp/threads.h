#pragma once

#include <string_view>

namespace lux_sp {

class Threads {
public:
  Threads() = delete;
  Threads(const Threads &) = delete;
  Threads(Threads &&) = delete;
  Threads &operator=(const Threads &) = delete;
  Threads &operator=(Threads &&) = delete;
  ~Threads() = delete;

  [[nodiscard]] static bool SetThreadAffinityToCore(int core_id) noexcept;

  // TODO: make it a header only library!!

  // template <typename Fn, typename... Args>
  // [[nodiscard]] static auto CreateAndStartThread(int core_id,
  //                                                std::string_view name, Fn &&fn,
  //                                                Args &&...args) noexcept;
};

} // namespace lux_sp
