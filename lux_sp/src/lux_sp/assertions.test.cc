#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <lux_sp/assertions.h>
#include <lux_sp/no_return_mock.h>

using ::testing::StrictMock;

namespace lux_sp {

class TestLuxSpAssertions : public testing::Test {
 protected:
  void SetUp() override {
    no_return_mock_ = std::make_unique<StrictMock<NoReturnMock>>();
  }
  std::unique_ptr<NoReturnMock> no_return_mock_;
};

TEST_F(TestLuxSpAssertions, AssertWhenConditionHolds) {
  // function under test
  Assertions{std::move(no_return_mock_)}.Assert(true, "no error");
}

TEST_F(TestLuxSpAssertions, AssertWhenConditionDoesNotHold) {
  EXPECT_DEATH(
      // function under test
      Assertions{std::move(no_return_mock_)}.Assert(false, "simulated error"),
      "simulated error");
}

TEST_F(TestLuxSpAssertions, FatalError) {
  EXPECT_DEATH(
      // function under test
      Assertions{std::move(no_return_mock_)}.FatalError("simulated error"),
      "fatal error: simulated error");
}

}  // namespace lux_sp
