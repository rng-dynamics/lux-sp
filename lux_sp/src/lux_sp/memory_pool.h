#pragma once

#include <lux_sp/utility.h>
#include <lux_sp/utility/overload.h>

#include <cstdint>
#include <variant>
#include <vector>

namespace lux_sp {

namespace memory_pool {}

template <typename T>
class MemoryPool final {
 private:
  struct Entry {
    T value_{};
    bool is_free_ = true;
  };

  constexpr static std::size_t offset_of_value_in_entry =
      offsetof(Entry, value_);
  static_assert(offset_of_value_in_entry == 0,
                "T value should be first entry in memory pool entry");

 public:
  explicit MemoryPool(std::size_t capacity) : store_{capacity, {T{}, true}} {
    // TODO: precondition: capacity > 0
  }
  MemoryPool() = delete;
  MemoryPool(const MemoryPool &) = delete;
  MemoryPool(MemoryPool &&) noexcept = default;
  MemoryPool &operator=(const MemoryPool &) = delete;
  MemoryPool &operator=(MemoryPool &&) noexcept = default;

  struct Success {
    T *value_{};
  };
  struct OutOfMemoryError {};

  template <typename... Args>
  std::variant<Success, OutOfMemoryError> Allocate(Args... args) noexcept {
    auto entry = &store_[next_free_index_];
    Utility::Assert(entry->is_free_,
                    "logic error: memory pool entry is not free");
    T *value = &(entry->value_);
    value = new (value) T{args...};  // placement new
    entry->is_free_ = false;

    if (auto success = UpdateNextFreeIndex(); !success) [[unlikely]] {
      return OutOfMemoryError{};
    }
    return Success{value};
  }

 private:
  bool UpdateNextFreeIndex() noexcept {
    const auto initial_free_index = next_free_index_;
    while (!store_[next_free_index_].is_free_) {
      ++next_free_index_;
      if (next_free_index_ == store_.size()) [[unlikely]] {
        next_free_index_ = 0;
      }
      if (initial_free_index == next_free_index_) [[unlikely]] {
        // TODO: out of memory strategy?
        return false;
      }
    }
    return true;
  }

  // TODO: change from std::vector to std::array.
  std::vector<Entry> store_{};
  uint64_t next_free_index_ = 0;
};

}  // namespace lux_sp
