#pragma once

namespace lux_sp {

class Exit {
 public:
  virtual ~Exit() = default;

  [[noreturn]] virtual void QuickExit(int status) const = 0;
};

}  // namespace lux_sp
