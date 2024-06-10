#pragma once

#include <cstddef>

#include <gmock/gmock.h>

#include <lux_sp/system.h>

namespace lux_sp {

class SystemMock : public System {
 public:
  // google mock may throw an exception
  // NOLINTNEXTLINE(bugprone-exception-escape)
  MOCK_METHOD(int, pthread_setaffinity_np,
              (pthread_t, std::size_t, cpu_set_t *),
              (const, noexcept, override));
};

}  // namespace lux_sp
