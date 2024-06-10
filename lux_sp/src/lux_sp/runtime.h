#pragma once

namespace lux_sp {

class Runtime {
 public:
  virtual ~Runtime() = default;

  virtual void terminate() const noexcept = 0;
};

}  // namespace lux_sp
