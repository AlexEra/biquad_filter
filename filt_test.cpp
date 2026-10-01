#include <iostream>
#include <fstream>
#include <cmath>
#include <numbers>
#include <array>
#include <random>
#include "biquad_filter.hpp"

using BiquadFilter::BiquadCascades;

int main() {
  const double dt{0.001}; // seconds
  const double f_sampling = 1.0 / dt; // Hz
  const double f_signal = 60; // Hz
  const double amp = 1.0;
  const double max_noise_percentage = 0.1;
  const double max_amp_noise = amp * max_noise_percentage;

  std::random_device seed;
  std::array<double, 1000> data;
  std::array<double, 1000> f_data;
  BiquadCascades<double, 3> biq;

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

  // TODO: setup filter
  // scipy.signal.butter(3, 100, output='sos', fs=1000)
  /*
  array([[ 3.40537653e-04,  6.81075305e-04,  3.40537653e-04,
         1.00000000e+00, -1.03206941e+00,  2.75707942e-01],
       [ 1.00000000e+00,  2.00000000e+00,  1.00000000e+00,
         1.00000000e+00, -1.14298050e+00,  4.12801598e-01],
       [ 1.00000000e+00,  2.00000000e+00,  1.00000000e+00,
         1.00000000e+00, -1.40438489e+00,  7.35915191e-01]])
  */
  biq.set_coefficients(
   {
    // {
      3.40537653e-04,  6.81075305e-04,  3.40537653e-04,
      1.00000000e+00, -1.03206941e+00,  2.75707942e-01,
    // },
    // {
      1.00000000e+00,  2.00000000e+00,  1.00000000e+00,
      1.00000000e+00, -1.14298050e+00,  4.12801598e-01,
    // },
    // {
      1.00000000e+00,  2.00000000e+00,  1.00000000e+00,
      1.00000000e+00, -1.40438489e+00,  7.35915191e-01
    // }
  }
  );
  // biq.start();

  // open file to save filtered data
  std::ofstream file("data.txt", std::ofstream::binary);
  // save header
  file << 'n' << '\t' << "s_0" << '\t' << "s_1" << "\r\n";

  /* filtering */
  for (auto i{0}; i < data.size(); i++) {
    f_data[i] = biq.step(data[i]);
    // save data to file
    file << i << '\t' << data[i] << '\t' << f_data[i] << "\r\n";
  }
  file.close();

  /* plot */
  /* system("plot \"data.txt\" using 1:2 with lines title \"raw\", \\
    \"data.txt\" using 1:3 with lines title \"filtered\""); */
  return 0;
}
