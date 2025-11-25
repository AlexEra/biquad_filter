#pragma once

#include <cstddef>
#include <array>
#include <concepts>

namespace BiquadFilter {

enum class Status {
  // s=-1,
  OK=0,
};

template<typename T>
concept filter_type = requires (T value) {
  value + value;
  value - value;
  value * value;
  value / value;
};

template<filter_type T, size_t SectionsCount>
class BiquadCascades final {
public:
  BiquadCascades(void) { };
  BiquadCascades(BiquadCascades&) = delete;
  BiquadCascades& operator=(const BiquadCascades&) = delete;

  Status set_coefficients(std::array< std::array<T, 6>, SectionsCount> &&array) {
    sos = array;
    return Status::OK;
  }

  Status start(void) {
    for (auto &value : delayed_values) {
      value = 0;
    }
    return Status::OK;
  }

  T step(T new_value) {
    // на примере 3 каскадов
    // 1st section: x_n * b_0_0 + x_0 * b_0_1 + x_1 * b_0_2 + x_2 * a_0_1 + x_3 * a_0_2
    // 2nd section: (1st_section * b_1_0) + x_2 * b_1_1 + x_3 * b_1_2 + x_4 * a_1_1 + x_5 * a_1_2
    // 3d section: (2nd_section * b_2_0) + x_4 * b_2_1 + x_5 * b_2_2 + x_6 * a_2_1 + x_7 * a_2_2 = y_n
    
    // section_result = new_value * b_0_0 + ....;
    // for (auto i{0}; i < SectionsCount-1;i ++) {
    //   section_result = (section_result * b_..._...) + ....;
    // }
    /** TODO: update delayed values using std::swap*/
    return new_value;
  }
private:
  T section_result{0};
  std::array<std::array<T, 6>, SectionsCount> sos{0}; // second order sections
  std::array<T, 4 + (SectionsCount - 1) * 2> delayed_values; // 4 delays from first sections, than +2 for each next section
};

} /* BiquadFilter */
