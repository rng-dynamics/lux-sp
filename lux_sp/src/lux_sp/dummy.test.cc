#include <gtest/gtest.h>

#include <lux_sp/dummy.h>

namespace lux_sp {

class TestDummy : public testing::Test {
public:
  TestDummy() noexcept = default;
  ~TestDummy() noexcept override = default;
  TestDummy(const TestDummy &) = delete;
  TestDummy(TestDummy &&) = delete;
  TestDummy &operator=(const TestDummy &) = delete;
  TestDummy &operator=(TestDummy &&) = delete;
};

TEST_F(TestDummy, Instantiation) { [[maybe_unused]] auto dummy = Dummy{}; }

} // namespace lux_sp
