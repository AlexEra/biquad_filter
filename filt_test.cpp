#include <iostream>
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

  /* filtering */
  for (auto i{0}; i < data.size(); i++) {
    f_data[i] = biq.step(data[i]);
  }

  // save data to file // TODO: implement

  
  /* plot */
  // TODO: implement
  // system("echo System call\r\n"); // TODO: use this function to call GNUPlot API

  return 0;
}
