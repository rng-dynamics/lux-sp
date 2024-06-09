#pragma once

#include <gmock/gmock.h>

#include <lux_sp/system.h>

namespace lux_sp {

class SystemMock : public System {
public:
  MOCK_METHOD(int, pthread_setaffinity_np,
              (pthread_t th, size_t cpusetsize, cpu_set_t *cpuset),
              (const, noexcept));
};

} // namespace lux_sp
