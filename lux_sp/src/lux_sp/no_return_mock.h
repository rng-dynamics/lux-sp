#pragma once

#include <cstdlib>

#include <lux_sp/no_return.h>

// TODO(alexander): add #ifdef GCOV
extern "C" void __gcov_dump();

namespace lux_sp {

// Note: The purpose of this mock is to call __gcov_dump() on before the
// process dies.
class NoReturnMock : public NoReturn {
 public:
  ~NoReturnMock() override = default;

  [[noreturn]] void QuickExit(int status) const override {
    // TODO(alexander): add #ifdef GCOV
    __gcov_dump();
    std::quick_exit(status);
  }
};

}  // namespace lux_sp
