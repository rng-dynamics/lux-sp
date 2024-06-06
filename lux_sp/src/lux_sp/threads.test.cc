#include <gtest/gtest.h>

#include <lux_sp/threads.h>

namespace lux_sp {

class TestLuxSpThreads : public testing::Test {
public:
  TestLuxSpThreads() noexcept = default;
  ~TestLuxSpThreads() noexcept override = default;
  TestLuxSpThreads(const TestLuxSpThreads &) = delete;
  TestLuxSpThreads(TestLuxSpThreads &&) = delete;
  TestLuxSpThreads &operator=(const TestLuxSpThreads &) = delete;
  TestLuxSpThreads &operator=(TestLuxSpThreads &&) = delete;
};

TEST_F(TestLuxSpThreads, SetThreadAffinityToCorePasses) {
  auto thread = std::thread([] {
    // function under test
    bool is_success = Threads::SetThreadAffinityToCore(0);

    ASSERT_TRUE(is_success);
  });
  thread.join();
}

} // namespace lux_sp
