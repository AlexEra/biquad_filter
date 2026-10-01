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
  std::array<double, 1000> data;
  std::array<double, 1000> f_data;
  BiquadCascades<double, 3> biq;

  /* preparing */
  {
    // fill data array
    double t{0};
    double noise{0}; // TODO: use rand or something else
    for (auto &value : data) {
      value = amp * sin(2 * std::numbers::pi * f_signal * t) + noise;
      t += dt;
      // noise = ; // TODO: use rand or something else
    }
  }

  // TODO: setup filter
  // biq.set_coefficients();
  // biq.start();

  // open file to save filtered data
  std::ofstream file("data.txt", std::ofstream::binary);
  // save header
  file << 'n' << "s_0" << "s_1" << "\r\n";

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
