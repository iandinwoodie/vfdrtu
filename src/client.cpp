// Local Headers
#include "client.h"
#include "config.h"

// Third Party Headers
#include <modbus.h>

// C++ Standard Library Headers
#include <iostream>
#include <memory>

#define TIMEOUT_SEC 10
#define TIMEOUT_USEC 0

namespace vfdrtu {
namespace {
class ClientImpl : public Client {
public:
  ClientImpl(const Config &config) : ctx_(nullptr), config_(config) {}

  ~ClientImpl() {
    modbus_close(ctx_);
    modbus_free(ctx_);
  }

  void Connect() {
    ctx_ = modbus_new_rtu(config_.port.c_str(), config_.baud, config_.parity,
                          config_.data_bits, config_.stop_bits);
    if (!ctx_) {
      throw std::runtime_error("modbus_new_rtu error");
    }

    modbus_set_response_timeout(ctx_, TIMEOUT_SEC, TIMEOUT_USEC);
#ifndef NDEBUG
    modbus_set_debug(ctx_, 1);
#endif

    if (SetServer(config_.device_id) < 0) {
      throw std::runtime_error("modbus_set_slave error");
    }

    if (modbus_connect(ctx_) < 0) {
      throw std::runtime_error("modbus_connect error");
    }
  }

  int SetServer(int server) override final {
    return modbus_set_slave(ctx_, server);
  }

  int WriteRegister(int addr, const uint16_t value) override final {
    return modbus_write_register(ctx_, addr, value);
  }

private:
  modbus_t *ctx_;
  const Config config_;
};
} // namespace

std::unique_ptr<Client> Client::Create(const Config &config) {
  auto client = std::make_unique<ClientImpl>(config);
  client->Connect();
  return client;
}
} // namespace vfdrtu
