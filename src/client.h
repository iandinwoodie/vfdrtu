#ifndef VFDRTU_CLIENT_H
#define VFDRTU_CLIENT_H

// Local Headers
#include "config.h"

// C++ Standard Library Headers
#include <memory>

namespace vfdrtu {
class Client {
public:
  // Factory method to create a Client instance.
  static std::unique_ptr<Client> Create(const Config &config);

  virtual ~Client() = default;
  virtual int SetServer(int server) = 0;
  virtual int WriteRegister(int addr, const uint16_t value) = 0;
};
} // namespace vfdrtu

#endif // VFDRTU_CLIENT_H
