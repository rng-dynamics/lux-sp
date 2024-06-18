#include <memory>
#include <utility>

#include <gtest/gtest.h>

#include <lux_sp/assertions.h>
#include <lux_sp/no_return_mock.h>

namespace lux_sp {

class TestLuxSpAssertions : public testing::Test {
 protected:
  void SetUp() override {
    no_return_mock_ = std::make_unique<NoReturnMock>();
  }

  std::unique_ptr<NoReturnMock> no_return_mock_;
};

// Note: Any death-test should be called ".*DeathTest".
// See:
// https://github.com/google/googletest/blob/main/docs/advanced.md#death-tests-and-threads
class TestLuxSpAssertionsDeathTest : public TestLuxSpAssertions {};

TEST_F(TestLuxSpAssertions, AssertWhenConditionHolds) {
  // function under test
  Assertions{std::move(no_return_mock_)}.Assert(true, "no error");
}

TEST_F(TestLuxSpAssertionsDeathTest, AssertWhenConditionDoesNotHold) {
  EXPECT_DEATH(
      // function under test
      Assertions{std::move(no_return_mock_)}.Assert(false, "simulated error"),
      "simulated error");
}

TEST_F(TestLuxSpAssertionsDeathTest, FatalError) {
  EXPECT_DEATH(
      // function under test
      Assertions{std::move(no_return_mock_)}.FatalError("simulated error"),
      "fatal error: simulated error");
}

}  // namespace lux_sp
