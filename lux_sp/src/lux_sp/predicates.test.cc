#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <lux_sp/exit_mock.h>
#include <lux_sp/predicates.h>

using ::testing::StrictMock;

namespace lux_sp {

class TestLuxSpPredicates : public testing::Test {
 protected:
  void SetUp() override {
    exit_mock_ = std::make_unique<StrictMock<ExitMock>>();
  }
  std::unique_ptr<ExitMock> exit_mock_;
};

TEST_F(TestLuxSpPredicates, AssertWhenConditionHolds) {
  // function under test
  Predicates{std::move(exit_mock_)}.Assert(true, "no error");
}

TEST_F(TestLuxSpPredicates, AssertWhenConditionDoesNotHold) {
  EXPECT_DEATH(
      // function under test
      Predicates{std::move(exit_mock_)}.Assert(false, "simulated error"),
      "simulated error");
}

TEST_F(TestLuxSpPredicates, FatalError) {
  EXPECT_DEATH(
      // function under test
      Predicates{std::move(exit_mock_)}.FatalError("simulated error"),
      "fatal error: simulated error");
}

}  // namespace lux_sp
