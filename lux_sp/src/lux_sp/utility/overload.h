#pragma once

namespace lux_sp::utility {

template <typename... Ts>
struct Overload : Ts... {
  using Ts::operator()...;
};

}  // namespace lux_sp::utility
