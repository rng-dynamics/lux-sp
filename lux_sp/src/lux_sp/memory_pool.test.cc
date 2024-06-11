#include <cstdint>
#include <cstdlib>
#include <memory>
#include <optional>
#include <utility>

#include <gtest/gtest.h>

#include <lux_sp/invariants.h>
#include <lux_sp/memory_pool.h>

namespace lux_sp {

class TestLuxSpMemoryPool : public testing::Test {
 protected:
  void SetUp() override {
    invariants_ = std::make_unique<Invariants>();
  }

  constexpr static std::uint64_t memory_pool_size_ = 1024;
  // we use protected member variables in test fixtures.
  // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes)
  std::unique_ptr<Invariants> invariants_;
};

struct SomeType {
  int content_{};
};

TEST_F(TestLuxSpMemoryPool, CreateNewPasses) {
  auto memory_pool =
      MemoryPool<SomeType>{std::move(invariants_), memory_pool_size_};
  const int int_value = 42;

  // function under test
  const auto result = memory_pool.CreateNew(int_value);

  ASSERT_TRUE(result.has_value());
  // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
  EXPECT_EQ(int_value, (*result)->content_);
}

TEST_F(TestLuxSpMemoryPool, DeletePasses) {
  auto memory_pool =
      MemoryPool<SomeType>{std::move(invariants_), memory_pool_size_};
  const int int_value = 42;
  std::optional<SomeType *> instance = memory_pool.CreateNew(int_value);
  if (!instance) {
    FAIL();
  }

  // function under test
  memory_pool.Delete(*instance);
}

TEST_F(TestLuxSpMemoryPool, DeleteWithNullptrFails) {
  auto memory_pool =
      MemoryPool<SomeType>{std::move(invariants_), memory_pool_size_};

  EXPECT_EXIT(
      // function under test
      memory_pool.Delete(nullptr),
      [](int exit_value) { return exit_value != EXIT_SUCCESS; },
      "deallocation request for nullptr");
}

TEST_F(TestLuxSpMemoryPool, DeleteWithForeignMemoryAddressFails) {
  auto memory_pool =
      MemoryPool<SomeType>{std::move(invariants_), memory_pool_size_};
  const int int_value = 42;
  std::optional<SomeType *> instance = memory_pool.CreateNew(int_value);
  if (!instance) {
    FAIL();
  }

  SomeType *low_address = *instance - 1;
  // Note: memory pool entries are bigger than the instances
  SomeType *high_address = *instance + memory_pool_size_ * 2;
  auto cases = std::array{low_address, high_address};

  for (auto &address : cases) {
    EXPECT_EXIT(
        // function under test
        memory_pool.Delete(address),
        [](int exit_value) { return exit_value != EXIT_SUCCESS; },
        "deallocation request does not belong to this memory pool");
  }
}

TEST_F(TestLuxSpMemoryPool, DeleteWithInvalidMemoryAddressFails) {
  auto memory_pool =
      MemoryPool<SomeType>{std::move(invariants_), memory_pool_size_};
  const int int_value = 42;
  std::optional<SomeType *> instance = memory_pool.CreateNew(int_value);
  if (!instance) {
    FAIL();
  }
  memory_pool.Delete(*instance);

  EXPECT_EXIT(
      // function under test
      memory_pool.Delete(*instance),
      [](int exit_value) { return exit_value != EXIT_SUCCESS; },
      "deallocation request of invalid pointer");
}

}  // namespace lux_sp
