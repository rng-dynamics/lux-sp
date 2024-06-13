#pragma once

#include <cstdlib>

#include <lux_sp/no_return.h>

extern "C" void __gcov_dump();

namespace lux_sp {

class NoReturnMock : public NoReturn {
 public:
  ~NoReturnMock() override = default;

  [[noreturn]] void QuickExit(int status) const override {
    __gcov_dump();
    std::quick_exit(status);
  }
};

}  // namespace lux_sp
