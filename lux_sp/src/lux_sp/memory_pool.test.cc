#include <cstdint>
#include <memory>
#include <utility>

#include <gtest/gtest.h>

#include <lux_sp/invariants.h>
#include <lux_sp/memory_pool.h>
#include <lux_sp/runtime_mock.h>

namespace lux_sp {

using ::testing::StrictMock;

class TestLuxSpMemoryPool : public testing::Test {
 protected:
  void SetUp() override {
    invariants_ = std::make_unique<Invariants>(
        std::make_unique<StrictMock<RuntimeMock>>());
  }

  constexpr static std::uint64_t memory_pool_size_ = 1024;
  // we use protected member variables in test fixtures.
  // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes)
  std::unique_ptr<Invariants> invariants_;
};

struct SomeType {
  int content_{};
};

TEST_F(TestLuxSpMemoryPool, Instantiation) {
  // function under test
  auto memory_pool =
      MemoryPool<SomeType>{std::move(invariants_), memory_pool_size_};
}

TEST_F(TestLuxSpMemoryPool, AllocatePasses) {
  auto memory_pool =
      MemoryPool<SomeType>{std::move(invariants_), memory_pool_size_};
  const int int_value = 42;

  // function under test
  const auto result = memory_pool.New(int_value);

  ASSERT_TRUE(result.has_value());
  // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
  EXPECT_EQ(int_value, (*result)->content_);
}

}  // namespace lux_sp
