#pragma once

#include <gmock/gmock.h>

#include <lux_sp/runtime.h>

namespace lux_sp {

class RuntimeMock : public Runtime {
 public:
  // google mock may throw an exception
  // NOLINTNEXTLINE(bugprone-exception-escape)
  MOCK_METHOD(void, terminate, (), (const, noexcept, override));
};

}  // namespace lux_sp
