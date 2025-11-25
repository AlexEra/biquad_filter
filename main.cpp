#include <iostream>
#include <format>
#include "biquad_filter.hpp"

int main() {
  BiquadFilter::BiquadCascades<double, 2> sos;
  sos.set_coefficients(
    // std::array<std::array<double, 6>, 2> 
    // {
      // std::array<double, 6> (1),
      // std::array<double, 6> {2, 6}
      {0,0,0,0,0,0,
      1,1,1,1,1,1}
    // }
  );
  sos.start();
  std::cout << sos.step(66.6) << '\n';

  // for (auto c_line : sos.sos) {
  //   for (auto coef : c_line) {
  //     std::cout << coef << '\n';
  //   }
  // }
  int v = 666;
  int w = 146;
  std::cout << std::format("Before v = {}, w = {}\n", v, w);
  std::swap(v, w);
  std::cout << std::format("After v = {}, w = {}\n", v, w);
  return 0;
}