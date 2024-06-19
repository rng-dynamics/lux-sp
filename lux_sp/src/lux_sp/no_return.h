#pragma once

namespace lux_sp {

class NoReturn {
 public:
  virtual ~NoReturn() = default;

  [[noreturn]] virtual void QuickExit(int status) const = 0;
};

}  // namespace lux_sp
