#ifndef VFDRTU_SERVER_H
#define VFDRTU_SERVER_H

// Local Headers
#include "config.h"

// C++ Standard Library Headers
#include <memory>

namespace vfdrtu {
class Server {
public:
  // Factory method to create a Server instance.
  static std::unique_ptr<Server> Create(const Config &config);

  virtual ~Server() = default;
  virtual int SetServer(int server) = 0;
  virtual void Start() = 0;
  virtual void Stop() = 0;
};
} // namespace vfdrtu

#endif // VFDRTU_SERVER_H
