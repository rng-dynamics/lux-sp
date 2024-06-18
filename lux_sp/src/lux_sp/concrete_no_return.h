#pragma once

#include <cstdlib>

#include <lux_sp/no_return.h>

namespace lux_sp {

class ConcreteNoReturn final : public NoReturn {
 public:
  [[noreturn]] void QuickExit(int status) const override {
    std::quick_exit(status);
  }
};

}  // namespace lux_sp
