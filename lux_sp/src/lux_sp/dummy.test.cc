#include <gtest/gtest.h>

#include <lux_sp/dummy.h>

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

TEST_F(TestLuxSpThreads, Instantiation) { [[maybe_unused]] auto dummy = Dummy{}; }

} // namespace lux_sp
