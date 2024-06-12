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
  EXPECT_EXIT(
      // function under test
      Predicates::Assert(false, "simulated error"),
      ::testing::KilledBySignal(SIGABRT), "simulated error");
}

TEST_F(TestLuxSpPredicates, FatalError) {
  EXPECT_EXIT(
      // function under test
      Predicates::FatalError("simulated error"),
      ::testing::KilledBySignal(SIGABRT), "fatal error: simulated error");
}

}  // namespace lux_sp
