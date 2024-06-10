#include <cstdint>

#include <gtest/gtest.h>

#include <lux_sp/memory_pool.h>
#include <lux_sp/utility/free_functions.h>

namespace lux_sp {

using lux_sp::utility::free_functions::Get;
using lux_sp::utility::free_functions::Is;

class TestLuxSpMemoryPool : public testing::Test {
 protected:
  void SetUp() override {}

  constexpr static std::uint64_t memory_pool_size_ = 1024;
};

struct SomeType {
  int content_{};
};

TEST_F(TestLuxSpMemoryPool, Instantiation) {
  // function under test
  auto memory_pool = MemoryPool<SomeType>{memory_pool_size_};
}

TEST_F(TestLuxSpMemoryPool, AllocatePasses) {
  auto memory_pool = MemoryPool<SomeType>{memory_pool_size_};
  const int int_value = 42;

  // function under test
  auto result = memory_pool.Allocate(int_value);

  EXPECT_TRUE(Is<MemoryPool<SomeType>::AllocationSuccess>(result));
  EXPECT_EQ(
      int_value,
      Get<MemoryPool<SomeType>::AllocationSuccess>(result).value_->content_);
}

}  // namespace lux_sp
