#include <gtest/gtest.h>

#include <lux_sp/spsc_lock_free_queue.h>

namespace lux_sp {

class TestLuxSpSpscLockFreeQueue : public testing::Test {};

struct SomeType {
  int content_{};
};

constexpr std::int64_t kCapacity = 8;

TEST_F(TestLuxSpSpscLockFreeQueue, Instantiation) {
  // function under test
  const auto _ = SpscLockFreeQueue<SomeType, kCapacity>{};
}

}  // namespace lux_sp
