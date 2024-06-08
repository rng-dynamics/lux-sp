#pragma once

#include <atomic>
#include <memory>
#include <thread>

#include <lux_sp/system.h>

namespace lux_sp {

class ThreadSupport {
public:
  explicit ThreadSupport(std::unique_ptr<System> system)
      : system_{std::move(system)} {}
  ThreadSupport(const ThreadSupport &) = delete;
  ThreadSupport(ThreadSupport &&) = default;
  ThreadSupport &operator=(const ThreadSupport &) = delete;
  ThreadSupport &operator=(ThreadSupport &&) = default;

  [[nodiscard]] bool SetThreadAffinityToCore(int core_id) noexcept {
    auto cpu_set = cpu_set_t{};
    CPU_ZERO(&cpu_set);
    CPU_SET(core_id, &cpu_set);
    return system_->pthread_setaffinity_np(::pthread_self(), sizeof(cpu_set_t),
                                           &cpu_set) == 0;
  }

  template <typename Fn, typename... Args>
  [[nodiscard]] std::unique_ptr<std::thread> CreateAndStartThread(int core_id,
                                          Fn &&fn, Args &&...args) noexcept {
    auto is_running = std::atomic_bool{false};
    auto is_failed = std::atomic_bool{false};

    auto thread_body = [&] {
      if (core_id >= 0 && !SetThreadAffinityToCore(core_id)) {
        is_failed.store(true, std::memory_order_release);
        return;
      }
      is_running.store(true, std::memory_order_release);
      std::forward<Fn>(fn)((std::forward<Args>(args))...);
    };

    auto thread = std::make_unique<std::thread>(thread_body);

    while (!is_running.load(std::memory_order_acquire) &&
           !is_failed.load(std::memory_order_acquire)) {
      using namespace std::literals::chrono_literals;
      std::this_thread::sleep_for(100ms);
    }

    if (is_failed) {
      thread->join();
    }
    return thread;
  }

private:
  std::unique_ptr<System> system_{};
};

} // namespace lux_sp
