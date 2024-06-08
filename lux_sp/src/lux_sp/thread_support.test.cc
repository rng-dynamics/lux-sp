#include <sched.h>

#include <gtest/gtest.h>

#include <lux_sp/system_mock.h>
#include <lux_sp/thread_support.h>

namespace lux_sp {

using ::testing::Return;
using ::testing::StrictMock;

class TestLuxSpThreadSupport : public testing::Test {
protected:
  void SetUp() override {
    system_mock_ = std::make_unique<StrictMock<SystemMock>>();
  }

  std::unique_ptr<SystemMock> system_mock_{};
};

TEST_F(TestLuxSpThreadSupport, SetThreadAffinityToCorePasses) {
  EXPECT_CALL(*system_mock_, pthread_setaffinity_np)
      .WillOnce(
          [](pthread_t th, size_t /* cpusetsize */, cpu_set_t *cpuset) -> int {
            EXPECT_EQ(::pthread_self(), th);
            EXPECT_EQ(1, CPU_COUNT(cpuset));
            EXPECT_TRUE(CPU_ISSET(0, cpuset));
            return 0;
          });
  auto threads = ThreadSupport{std::move(system_mock_)};

  // function under test
  bool is_success = threads.SetThreadAffinityToCore(0);

  ASSERT_TRUE(is_success);
}

TEST_F(TestLuxSpThreadSupport,
       SetThreadAffinityToCoreWhenCoreIdentifierIsNegativeFails) {
  EXPECT_CALL(*system_mock_, pthread_setaffinity_np)
      .WillOnce(
          [](pthread_t th, size_t /* cpusetsize */, cpu_set_t *cpuset) -> int {
            EXPECT_EQ(::pthread_self(), th);
            EXPECT_EQ(0, CPU_COUNT(cpuset));
            return -1;
          });
  auto threads = ThreadSupport{std::move(system_mock_)};

  // function under test
  bool is_success = threads.SetThreadAffinityToCore(-1);

  ASSERT_FALSE(is_success);
}

TEST_F(TestLuxSpThreadSupport, SetTrheadAffinityToCoreFails) {
  EXPECT_CALL(*system_mock_, pthread_setaffinity_np).WillOnce(Return(-1));
  auto threads = ThreadSupport{std::move(system_mock_)};

  // function under test
  bool is_success = threads.SetThreadAffinityToCore(0);

  ASSERT_FALSE(is_success);
}

} // namespace lux_sp