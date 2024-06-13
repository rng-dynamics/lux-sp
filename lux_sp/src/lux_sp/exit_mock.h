#pragma once

#include <cstdlib>

#include <lux_sp/exit.h>

extern "C" void __gcov_dump();

namespace lux_sp {

class ExitMock : public Exit {
 public:
  ~ExitMock() override = default;

  [[noreturn]] void QuickExit(int status) const override {
    __gcov_dump();
    std::quick_exit(status);
  }
};

}  // namespace lux_sp
