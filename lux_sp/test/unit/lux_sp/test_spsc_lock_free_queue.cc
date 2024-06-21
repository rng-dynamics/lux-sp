#include <gtest/gtest.h>

#include <lux_sp/spsc_lock_free_queue.h>

namespace lux_sp {

class TestLuxSpSpscLockFreeQueue : public testing::Test {};

struct SomeType {
  int content_{};
};

constexpr std::int64_t kCapacity = 8;

TEST_F(TestLuxSpSpscLockFreeQueue, NextEntryToWriteToHasValue) {
  auto queue = SpscLockFreeQueue<SomeType, kCapacity>{};

  // function under test
  const std::optional<SomeType *> value_ptr = queue.NextEntryToWriteTo();

  EXPECT_TRUE(value_ptr.has_value());
}

TEST_F(TestLuxSpSpscLockFreeQueue,
       WhenQueueIsFullThenNextEntryToWriteDoesNotHaveValue) {
  constexpr std::int64_t capacity = 1;
  auto queue = SpscLockFreeQueue<SomeType, capacity>{};
  **queue.NextEntryToWriteTo() = SomeType{42};
  queue.UpdateWriteIndex();

  // function under test
  const std::optional<SomeType *> value_ptr = queue.NextEntryToWriteTo();

  EXPECT_FALSE(value_ptr.has_value());
}

TEST_F(TestLuxSpSpscLockFreeQueue, UpdateWriteIndex) {
  auto queue = SpscLockFreeQueue<SomeType, kCapacity>{};

  // function under test
  queue.UpdateWriteIndex();

  EXPECT_EQ(1, queue.Size());
}

TEST_F(TestLuxSpSpscLockFreeQueue, NextEntryToRead) {
  auto queue = SpscLockFreeQueue<SomeType, kCapacity>{};
  **queue.NextEntryToWriteTo() = SomeType{42};
  queue.UpdateWriteIndex();

  // function under test
  const std::optional<SomeType *> value_ptr = queue.NextEntryToRead();

  EXPECT_TRUE(value_ptr.has_value());
}

TEST_F(TestLuxSpSpscLockFreeQueue,
       WhenQueueIsEmptyThenNextEntryToReadDoesNotHaveValue) {
  auto queue = SpscLockFreeQueue<SomeType, kCapacity>{};

  // function under test
  const std::optional<SomeType *> value_ptr = queue.NextEntryToRead();

  EXPECT_FALSE(value_ptr.has_value());
}

TEST_F(TestLuxSpSpscLockFreeQueue, UpdateReadIndex) {
  auto queue = SpscLockFreeQueue<SomeType, kCapacity>{};
  **queue.NextEntryToWriteTo() = SomeType{42};
  queue.UpdateWriteIndex();

  // function under test
  queue.UpdateReadIndex();

  EXPECT_TRUE(queue.IsEmpty());
}

TEST_F(TestLuxSpSpscLockFreeQueue, UpdateReadIndexWrapsAround) {
  constexpr std::int64_t capacity = 1;
  auto queue = SpscLockFreeQueue<SomeType, capacity>{};
  **queue.NextEntryToWriteTo() = SomeType{42};
  queue.UpdateWriteIndex();

  // function under test
  queue.UpdateReadIndex();

  EXPECT_TRUE(queue.IsEmpty());
}

TEST_F(TestLuxSpSpscLockFreeQueue, Capacity) {
  auto queue = SpscLockFreeQueue<SomeType, kCapacity>{};

  // function under test
  const std::int64_t capacity = queue.Capacity();

  EXPECT_EQ(kCapacity, capacity);
}

TEST_F(TestLuxSpSpscLockFreeQueue, Size) {
  auto queue = SpscLockFreeQueue<SomeType, kCapacity>{};
  **queue.NextEntryToWriteTo() = SomeType{42};
  queue.UpdateWriteIndex();

  // function under test
  const std::int64_t size = queue.Size();

  EXPECT_EQ(1, size);
}

TEST_F(TestLuxSpSpscLockFreeQueue, IsEmptyWhenEmpty) {
  auto queue = SpscLockFreeQueue<SomeType, kCapacity>{};

  // function under test
  const bool is_empty = queue.IsEmpty();

  EXPECT_TRUE(is_empty);
}

TEST_F(TestLuxSpSpscLockFreeQueue, NotIsEmptyWhenNotEmpty) {
  auto queue = SpscLockFreeQueue<SomeType, kCapacity>{};
  **queue.NextEntryToWriteTo() = SomeType{42};
  queue.UpdateWriteIndex();

  // function under test
  const bool is_empty = queue.IsEmpty();

  EXPECT_FALSE(is_empty);
}

TEST_F(TestLuxSpSpscLockFreeQueue, NotIsFullWhenNotFull) {
  auto queue = SpscLockFreeQueue<SomeType, kCapacity>{};

  // function under test
  const bool is_full = queue.IsFull();

  EXPECT_FALSE(is_full);
}

TEST_F(TestLuxSpSpscLockFreeQueue, IsFullWhenFull) {
  constexpr std::int64_t capacity = 1;
  auto queue = SpscLockFreeQueue<SomeType, capacity>{};
  **queue.NextEntryToWriteTo() = SomeType{42};
  queue.UpdateWriteIndex();

  // function under test
  const bool is_full = queue.IsFull();

  EXPECT_TRUE(is_full);
}

}  // namespace lux_sp
