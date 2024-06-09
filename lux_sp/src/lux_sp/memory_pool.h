#pragma once

#include <lux_sp/utility.h>
#include <lux_sp/utility/free_functions.h>
#include <lux_sp/utility/overload.h>

#include <cstdint>
#include <variant>
#include <vector>

namespace {}

namespace lux_sp {

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

  // TODO: these structs should go outside of this class
  struct AllocationSuccess {
    T *value_{};
  };
  struct OutOfMemoryError {};

  template <typename... Args>
  std::variant<AllocationSuccess, OutOfMemoryError> Allocate(
      Args... args) noexcept {
    auto entry = &store_[next_free_index_];
    Utility::Assert(entry->is_free_,
                    "logic error: memory pool entry is not free");
    T *item = &(entry->value_);
    item = new (item) T{args...};  // placement new
    entry->is_free_ = false;

    if (auto success = UpdateNextFreeIndex(); !success) [[unlikely]] {
      return OutOfMemoryError{};
    }
    return AllocationSuccess{item};
  }

  struct DeallocationSuccess {};
  struct DeallocationError {
    std::string_view message_{};
  };

  std::variant<DeallocationSuccess, DeallocationError> Deallocate(
      const T *item) noexcept {
    const Entry *entry = item - offset_of_value_in_entry;
    const std::ptrdiff_t entry_index = entry - &store_[0];
    Utility::Assert(0 <= entry_index && entry_index < store_.size(),
                    "deallocation request does not belong to this memory pool");
    Utility::Assert(!entry->is_free, "deallocation request of invalid pointer");
    entry->value_.~T();
    entry->is_free_ = true;
    return DeallocationSuccess{};
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
