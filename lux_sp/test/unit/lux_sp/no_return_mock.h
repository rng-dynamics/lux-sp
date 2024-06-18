#pragma once

#include <cstdlib>

#include <lux_sp/no_return.h>

#ifdef ENABLE_GCOV
// This function declaration references a reserved symbol. I.e., we have to use
// the reserved name which violates some lints.
// NOLINTNEXTLINE(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
extern "C" void __gcov_dump();
#endif

namespace lux_sp {

// Note: The purpose of this mock is to call __gcov_dump() just before the
// process dies.
class NoReturnMock : public NoReturn {
 public:
  ~NoReturnMock() override = default;

  [[noreturn]] void QuickExit(int status) const override {
#ifdef ENABLE_GCOV
    __gcov_dump();
#endif
    std::quick_exit(status);
  }
};

}  // namespace lux_sp
