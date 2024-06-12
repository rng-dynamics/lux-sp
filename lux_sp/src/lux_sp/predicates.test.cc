#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <lux_sp/predicates.h>

namespace lux_sp {

class TestLuxSpPredicates : public testing::Test {};

TEST_F(TestLuxSpPredicates, AssertWhenConditionHolds) {
  // function under test
  Predicates::Assert(true, "no error");
}

TEST_F(TestLuxSpPredicates, AssertWhenConditionDoesNotHold) {
  EXPECT_DEATH(
      // function under test
      Predicates::Assert(false, "simulated error"), "simulated error");
}

TEST_F(TestLuxSpPredicates, FatalError) {
  EXPECT_DEATH(
      // function under test
      Predicates::FatalError("simulated error"),
      "fatal error: simulated error");
}

}  // namespace lux_sp
