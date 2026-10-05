#include <iostream>
#include <fstream>
#include <cmath>
#include <numbers>
#include <array>
#include <random>
#include "biquad_filter.hpp"

// #define TEST_UNIT_CASCADES
#define TEST_SEPARATE_CASCADES

using BiquadFilter::BiquadCascades;

int main() {
  const double dt{0.001}; // seconds
  const double f_sampling = 1.0 / dt; // Hz
  const double f_signal = 60; // Hz
  const double amp = 1.0;
  const double max_noise_percentage = 0.36;
  const double max_amp_noise = amp * max_noise_percentage;

  std::random_device seed;
  std::array<double, 100> data;
  std::array<double, 100> f_data;

#ifdef TEST_UNIT_CASCADES
  BiquadCascades<double, 3> biq;
#elif defined(TEST_SEPARATE_CASCADES)
  BiquadFilter::BiquadSection<double> bs[3];
#else
#error "Test case isn't chosen!"
#endif

  /* setup noise generator */
  std::mt19937 generator(seed());
  std::uniform_real_distribution<double> distribution(-max_amp_noise, max_amp_noise);

  /* preparing */
  {
    // fill data array
    double t{0};
    double noise{distribution(generator)};
    for (auto &value : data) {
      noise = distribution(generator);
      value = amp * sin(2 * std::numbers::pi * f_signal * t) + noise;
      t += dt;
    }
  }

#ifdef TEST_UNIT_CASCADES
  biq.set_coefficients({
    /* sos 1 */
    3.40537653e-04,  6.81075305e-04,  3.40537653e-04,
    1.00000000e+00, -1.03206941e+00,  2.75707942e-01,
    /* sos 2 */
    1.00000000e+00,  2.00000000e+00,  1.00000000e+00,
    1.00000000e+00, -1.14298050e+00,  4.12801598e-01,
    /* sos 3 */
    1.00000000e+00,  2.00000000e+00,  1.00000000e+00,
    1.00000000e+00, -1.40438489e+00,  7.35915191e-01
  });
#elif defined(TEST_SEPARATE_CASCADES)
  bs[0].set_coefficients({
    3.40537653e-04,  6.81075305e-04,  3.40537653e-04,
    1.00000000e+00, -1.03206941e+00,  2.75707942e-01
  });
  bs[1].set_coefficients({
    1.00000000e+00,  2.00000000e+00,  1.00000000e+00,
    1.00000000e+00, -1.14298050e+00,  4.12801598e-01
  });
  bs[2].set_coefficients({
    1.00000000e+00,  2.00000000e+00,  1.00000000e+00,
    1.00000000e+00, -1.40438489e+00,  7.35915191e-01
  });
#endif

  // open file to save filtered data
  std::ofstream file("data.txt", std::ofstream::binary);
  // save header
  file << '#' << '\t' << 'n' << '\t' << "s_0" << '\t' << "s_1" << "\r\n";

  /* filtering */
  for (auto i{0}; i < data.size(); i++) {
#ifdef TEST_UNIT_CASCADES
    f_data[i] = biq.step(data[i]);
#elif defined(TEST_SEPARATE_CASCADES)
    f_data[i] = bs[0].step(data[i]);
    f_data[i] = bs[1].step(f_data[i]);
    f_data[i] = bs[2].step(f_data[i]);
#endif
    // save data to file
    file << i << '\t' << data[i] << '\t' << f_data[i] << "\r\n";
  }
  file.close();

  /* plot */
  // plot two columns
  system("gnuplot -e \"set grid; plot 'data.txt' using 1:2 with\
    lines title 'raw', 'data.txt' using 1:3 with lines\
    title 'filtered'; pause mouse close\""
  );

  // for one column
  // system("gnuplot -e \"set size ratio 0.5; set grid; plot 'data.txt' using 1:2 with lines title 'raw'; pause mouse close\"");
  return 0;
}
