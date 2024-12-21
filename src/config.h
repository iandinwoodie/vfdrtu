#ifndef VFDRTU_CONFIG_H
#define VFDRTU_CONFIG_H

// C++ Standard Library Headers
#include <string>

namespace vfdrtu {
struct Config {
  std::string port;
  int baud = 9600;
  char parity = 'N';
  int data_bits = 8;
  int stop_bits = 1;
  int device_id = 1;

  static void Print(const Config &config);
};
} // namespace vfdrtu

#endif // VFDRTU_CONFIG_H
