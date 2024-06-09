#include <gtest/gtest.h>

#include <lux_sp/memory_pool.h>

namespace lux_sp {

using namespace lux_sp::utility::free_functions;

class TestLuxSpMemoryPool : public testing::Test {
 protected:
  void SetUp() override {}

  constexpr static uint64_t memory_pool_size_ = 1024;
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

  // function under test
  auto result = memory_pool.Allocate(42);

  // TODO: we need some more convenience here
  EXPECT_TRUE(Is<MemoryPool<SomeType>::AllocationSuccess>(result));
  EXPECT_EQ(
      42,
      Get<MemoryPool<SomeType>::AllocationSuccess>(result).value_->content_);
}

}  // namespace lux_sp
