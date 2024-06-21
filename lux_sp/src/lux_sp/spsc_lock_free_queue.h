#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <optional>

namespace lux_sp {

// TODO(as): rename
template <typename T, std::uint64_t Cap>
  requires std::default_initializable<T>
class SpscLockFreeQueue final {
  static_assert(
      Cap > 0,
      "Given capacity value for lock-free queue is less or equal to zero.");

 public:
  SpscLockFreeQueue(const SpscLockFreeQueue &) = delete;
  SpscLockFreeQueue(SpscLockFreeQueue &&) = delete;
  SpscLockFreeQueue &operator=(const SpscLockFreeQueue &) = delete;
  SpscLockFreeQueue &operator=(SpscLockFreeQueue &&) = delete;
  SpscLockFreeQueue() noexcept = default;
  ~SpscLockFreeQueue() noexcept = default;

  [[nodiscard]] std::optional<T *> NextEntryToWriteTo() noexcept {
    if (IsFull()) [[unlikely]] {
      return {};
    }
    return &store_.at(write_index_);
  }

  void UpdateWriteIndex() noexcept {
    if (write_index_ + 1 >= std::ssize(store_)) [[unlikely]] {
      write_index_ = 0;
    } else {
      ++write_index_;
    }
    ++n_elements_;
  }

  [[nodiscard]] std::optional<T *> NextEntryToRead() noexcept {
    if (IsEmpty()) [[unlikely]] {
      return {};
    }
    return &store_.at(read_index_);
  }

  void UpdateReadIndex() noexcept {
    if (read_index_ + 1 >= std::ssize(store_)) [[unlikely]] {
      read_index_ = 0;
    } else {
      ++read_index_;
    }
    --n_elements_;
  }

  [[nodiscard]] constexpr std::int64_t Capacity() const noexcept {
    return std::ssize(store_);
  }

  [[nodiscard]] std::int64_t Size() const noexcept {
    return n_elements_;
  }

  [[nodiscard]] bool IsEmpty() const noexcept {
    return Size() == 0;
  }

  [[nodiscard]] bool IsFull() const noexcept {
    return Size() == Capacity();
  }

 private:
  std::array<T, Cap> store_{};
  std::atomic_int64_t write_index_{0};
  std::atomic_int64_t read_index_{0};
  std::atomic_int64_t n_elements_{0};
};

}  // namespace lux_sp
