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

/**
 * @brief 
 * @tparam T              Type of values to filter
 * @tparam SectionsCount  Amount of biquad sections
 */
template<filter_type T, size_t SectionsCount>
class BiquadCascades final {
public:
  Status set_coefficients(std::array< std::array<T, 6>, SectionsCount> &&array) {
    for (size_t i{0}; i < SectionsCount; i++) {
      for (size_t j{0}; j < 6; j++) {
        if (j == 3) { // value with this index isn't used in computations
          continue;
        }
        sos[i][(j > 3) ? j - 1 : j] = array[i][j];
      }
    }
    return Status::OK;
  }

  T step(T new_value) {
    // compute 1st section
    section_result = new_value * sos[0][0] + delayed_values[0] * sos[0][1] +
      delayed_values[1] * sos[0][2] - delayed_values[2] * sos[0][3] -
      delayed_values[3] * sos[0][4];
    // update delays for 1st section
    delayed_values[1] = delayed_values[0];
    delayed_values[0] = new_value;
    // next sections
    for (auto i{1}, j{2}; i < SectionsCount; i++, j += 2) {
      prev_section_result = section_result; // backup result
      // compute next section result
      section_result = section_result * sos[i][0] + delayed_values[j] * sos[i][1] +
        delayed_values[j + 1] * sos[i][2] - delayed_values[j + 2] * sos[i][3] -
        delayed_values[j + 3] * sos[i][4];
      // update delays
      delayed_values[j + 1] = delayed_values[j];
      delayed_values[j] = prev_section_result;
    }
    // update last two delayed values
    delayed_values[delayed_values.size() - 1] = delayed_values[delayed_values.size() - 2];
    delayed_values[delayed_values.size() - 2] = section_result;
    return section_result;
  }
private:
  T section_result{0};
  T prev_section_result{0};
  std::array<std::array<T, 5>, SectionsCount> sos{0}; // second order sections (a0 equals to 1, it isn't used)
  std::array<T, 4 + (SectionsCount - 1) * 2> delayed_values{0}; // 4 delays from first sections, than +2 for each next section
};

/**
 * @brief 
 * @tparam T Type of values to filter  
 */
template<filter_type T>
class BiquadSection final {
public:
  Status set_coefficients(std::array<T, 6> &&array) {
    /* TODO: save coefficients, that will be used, i.e. save all except array[i][3]
    It helps with memory consumption decreasing
    */
    sos = array;
    return Status::OK;
  }

  T step(T new_value) {
    T section_result = new_value * sos[0] + delayed_values[0] * sos[1] +
     delayed_values[1] * sos[2] - delayed_values[2] * sos[4] - delayed_values[3] * sos[5];
    // update delays
    delayed_values[3] = delayed_values[2];
    delayed_values[2] = section_result;
    delayed_values[1] = delayed_values[0];
    delayed_values[0] = new_value;
    return section_result;
  }
private:
  std::array<T, 6> sos{0,};
  std::array<T, 4> delayed_values{0,};
};

} /* BiquadFilter */
