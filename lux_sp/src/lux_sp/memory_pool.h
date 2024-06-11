#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include <lux_sp/invariants.h>
#include <lux_sp/utility/free_functions.h>
#include <lux_sp/utility/overload.h>

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
  MemoryPool(std::unique_ptr<Invariants> invariants, std::size_t capacity)
      : invariants_{std::move(invariants)}, store_{capacity, {T{}, true}} {
    // TODO(alexander): precondition: capacity > 0
  }
  MemoryPool() = delete;
  ~MemoryPool() {
    // TODO(alexander): if not all memory freed, fatal error.
    (void)nullptr;
  }
  MemoryPool(const MemoryPool &) = delete;
  MemoryPool(MemoryPool &&) noexcept = default;
  MemoryPool &operator=(const MemoryPool &) = delete;
  MemoryPool &operator=(MemoryPool &&) noexcept = default;

  template <typename... Args>
  [[nodiscard]] std::optional<T *> CreateNew(Args... args) noexcept {
    std::optional<uint64_t> next_free_index = ComputeNextFreeIndex();
    if (!next_free_index) [[unlikely]] {
      return {};
    }

    Entry *entry = &store_[free_index_];
    invariants_->Assert(entry->is_free_,
                        "logic error: memory pool entry is not free");
    T *item = &(entry->value_);
    new (item) T{args...};  // placement new
    entry->is_free_ = false;

    free_index_ = *next_free_index;
    return item;
  }

  void Delete(T *item) noexcept {
    // invariants_->Assert(item != nullptr, "deallocation request for nullptr");
    invariants_->Assert(item != nullptr, "deallocation request for nullptr");
    Entry *entry = From(item);
    const bool is_lower_bound_maintained = store_.data() <= entry;
    const bool is_upper_bound_maintained = entry <= &*store_.crbegin();
    constexpr auto assertion_message =
        "deallocation request does not belong to this memory pool";
    invariants_->Assert(is_lower_bound_maintained, assertion_message);
    invariants_->Assert(is_upper_bound_maintained, assertion_message);
    invariants_->Assert(!entry->is_free_,
                        "deallocation request of invalid pointer");
    entry->value_.~T();
    entry->is_free_ = true;
  }

 private:
  [[nodiscard]] Entry *From(T *value) {
    static_assert(offset_of_value_in_entry == 0,
                  "T value should be first entry in memory pool entry");
    // NOLINTNEXTLINE(bugprone-casting-through-void)
    return static_cast<Entry *>(static_cast<void *>(value));
  }

  [[nodiscard]] std::optional<std::uint64_t> ComputeNextFreeIndex()
      const noexcept {
    const std::uint64_t store_size = std::ssize(store_);
    auto index = free_index_;
    for (std::uint64_t loop_index = 0; loop_index < store_size; ++loop_index) {
      index += 1;
      if (index >= store_size) [[unlikely]] {
        index = 0;
      }
      if (store_[index].is_free_) {
        break;
      }
    }
    if (index == free_index_) [[unlikely]] {
      return {};
    }
    invariants_->Assert(store_[index].is_free_, "logic error");
    return index;
  }

  std::unique_ptr<Invariants> invariants_;
  // TODO(alexander): change from std::vector to std::array.
  std::vector<Entry> store_{};
  std::uint64_t free_index_ = 0;
};

}  // namespace lux_sp
