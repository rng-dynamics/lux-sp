#include <array>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <optional>
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <lux_sp/assertions.h>
#include <lux_sp/memory_pool.h>
#include <lux_sp/no_return_mock.h>

using ::testing::StrictMock;

namespace lux_sp {

class TestLuxSpMemoryPool : public testing::Test {
 protected:
  void SetUp() override {
    no_return_ = std::make_unique<StrictMock<NoReturnMock>>();
  }

  std::unique_ptr<NoReturnMock> no_return_;
  constexpr static std::int64_t memory_pool_size_ = 1024;
};

// Note: Any death-test should be called ".*DeathTest".
// See:
// https://github.com/google/googletest/blob/main/docs/advanced.md#death-tests-and-threads
class TestLuxSpMemoryPoolDeathTest : public TestLuxSpMemoryPool {};

struct SomeType {
  int content_{};
};

TEST_F(TestLuxSpMemoryPool, CreateNewPasses) {
  auto memory_pool = MemoryPool<SomeType, memory_pool_size_>{
      std::make_unique<Assertions>(std::move(no_return_))};
  const int int_value = 42;

  // function under test
  const auto result = memory_pool.CreateNew(int_value);

  ASSERT_TRUE(result.has_value());
  // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
  EXPECT_EQ(int_value, (*result)->content_);
}

TEST_F(TestLuxSpMemoryPool, CreateNewWhenCapacityExhaustedFails) {
  auto memory_pool = MemoryPool<SomeType, 2>{
      std::make_unique<Assertions>(std::move(no_return_))};
  const int int_value = 42;
  const auto result_1 = memory_pool.CreateNew(int_value);
  ASSERT_TRUE(result_1.has_value());

  // function under test
  const auto result = memory_pool.CreateNew(int_value);

  EXPECT_FALSE(result.has_value());
}

TEST_F(TestLuxSpMemoryPool, DeletePasses) {
  auto memory_pool = MemoryPool<SomeType, memory_pool_size_>{
      std::make_unique<Assertions>(std::move(no_return_))};
  const int int_value = 42;
  std::optional<SomeType *> instance = memory_pool.CreateNew(int_value);
  if (!instance) {
    FAIL();
  }

  // function under test
  memory_pool.Delete(*instance);
}

TEST_F(TestLuxSpMemoryPoolDeathTest, DeleteWithNullptrFails) {
  auto memory_pool = MemoryPool<SomeType, memory_pool_size_>{
      std::make_unique<Assertions>(std::move(no_return_))};

  EXPECT_DEATH(
      // function under test
      memory_pool.Delete(nullptr), "deallocation request for nullptr");
}

TEST_F(TestLuxSpMemoryPoolDeathTest, DeleteWithForeignMemoryAddressFails) {
  auto memory_pool = MemoryPool<SomeType, memory_pool_size_>{
      std::make_unique<Assertions>(std::move(no_return_))};
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
    EXPECT_DEATH(
        // function under test
        memory_pool.Delete(address),
        "deallocation request does not belong to this memory pool");
  }
}

TEST_F(TestLuxSpMemoryPoolDeathTest, DeleteWithInvalidMemoryAddressFails) {
  auto memory_pool = MemoryPool<SomeType, memory_pool_size_>{
      std::make_unique<Assertions>(std::move(no_return_))};
  const int int_value = 42;
  std::optional<SomeType *> instance = memory_pool.CreateNew(int_value);
  if (!instance) {
    FAIL();
  }
  memory_pool.Delete(*instance);

  EXPECT_DEATH(
      // function under test
      memory_pool.Delete(*instance), "deallocation request of invalid pointer");
}

}  // namespace lux_sp
