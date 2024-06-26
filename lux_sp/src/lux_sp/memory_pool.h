#pragma once

#include <array>
#include <cstdint>
#include <optional>

#include <lux_sp/assertions.h>

namespace lux_sp {

template <std::default_initializable T, std::int64_t capacity>
  requires(capacity > 0)
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
  explicit MemoryPool(std::unique_ptr<Assertions> assertions)
      : assertions_{std::move(assertions)} {}
  MemoryPool() = delete;
  ~MemoryPool() {
    for (auto &entry : store_) {
      if (!entry.is_free_) {
        Delete(&entry.value_);
      }
    }
  }
  MemoryPool(const MemoryPool &) = delete;
  MemoryPool(MemoryPool &&) noexcept = default;
  MemoryPool &operator=(const MemoryPool &) = delete;
  MemoryPool &operator=(MemoryPool &&) noexcept = default;

  template <typename... Args>
  [[nodiscard]] std::optional<T *> CreateNew(Args... args) noexcept {
    std::optional<int64_t> next_free_index = ComputeNextFreeIndex();
    if (!next_free_index) [[unlikely]] {
      return {};
    }

    Entry *entry = &store_.at(free_index_);
    T *item = &(entry->value_);
    new (item) T{args...};  // placement new
    entry->is_free_ = false;

    free_index_ = *next_free_index;
    return item;
  }

  void Delete(T *item) noexcept {
    assertions_->Assert(item != nullptr, "deallocation request for nullptr");
    Entry *entry = From(item);
    const bool is_lower_bound_maintained = store_.data() <= entry;
    const bool is_upper_bound_maintained = entry <= &*store_.crbegin();
    constexpr auto assertion_message =
        "deallocation request does not belong to this memory pool";
    assertions_->Assert(is_lower_bound_maintained, assertion_message);
    assertions_->Assert(is_upper_bound_maintained, assertion_message);
    assertions_->Assert(!entry->is_free_,
                        "deallocation request of invalid pointer");
    entry->value_.~T();
    entry->is_free_ = true;
  }

 private:
  [[nodiscard]] Entry *From(T *value) {
    static_assert(offset_of_value_in_entry == 0,
                  "T value should be first entry in memory pool entry");
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<Entry *>(value);
  }

  [[nodiscard]] std::optional<std::int64_t> ComputeNextFreeIndex()
      const noexcept {
    const std::int64_t store_size = std::ssize(store_);
    auto index = free_index_;
    for (std::int64_t loop_index = 0; loop_index < store_size; ++loop_index) {
      index += 1;
      if (index >= store_size) [[unlikely]] {
        index = 0;
      }
      if (store_.at(index).is_free_) {
        break;
      }
    }
    if (index == free_index_) [[unlikely]] {
      return {};
    }
    assertions_->Assert(store_.at(index).is_free_, "logic error");
    return index;
  }

  std::unique_ptr<Assertions> assertions_;
  std::array<Entry, capacity> store_{};
  std::int64_t free_index_ = 0;
};

}  // namespace lux_sp
