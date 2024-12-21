// Local Headers
#include "config.h"

// C++ Standard Library Headers
#include <iostream>
#include <string>

namespace vfdrtu {
void Config::Print(const Config &config) {
  std::cout << "Configuration:\n"
            << "  Port: " << config.port << "\n"
            << "  Baud: " << config.baud << "\n"
            << "  Parity: " << config.parity << "\n"
            << "  Data Bits: " << config.data_bits << "\n"
            << "  Stop Bits: " << config.stop_bits << "\n"
            << "  Device ID: " << config.device_id << std::endl;
}
} // namespace vfdrtu
