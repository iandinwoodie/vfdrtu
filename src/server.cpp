// Local Headers
#include "server.h"
#include "config.h"

// Third Party Headers
#include <modbus.h>

// C++ Standard Library Headers
#include <atomic>
#include <iostream>
#include <memory>
#include <thread>

namespace vfdrtu {
namespace {
std::atomic_bool exit_condition(false);

class ServerImpl : public Server {
public:
  ServerImpl(const Config &config)
      : ctx_(nullptr), mb_mapping_(nullptr), config_(config) {}

  ~ServerImpl() {
    modbus_mapping_free(mb_mapping_);
    modbus_close(ctx_);
    modbus_free(ctx_);
  }

  void Connect() {
    ctx_ = modbus_new_rtu(config_.port.c_str(), config_.baud, config_.parity,
                          config_.data_bits, config_.stop_bits);
    if (!ctx_) {
      throw std::runtime_error("modbus_new_rtu error");
    }

#ifndef NDEBUG
    modbus_set_debug(ctx_, 1);
#endif

    if (SetServer(config_.device_id) < 0) {
      throw std::runtime_error("modbus_set_slave error");
    }

    if (modbus_connect(ctx_) < 0) {
      throw std::runtime_error("modbus_connect error");
    }

    // Init the modbus mapping structure, will contain the data
    // that will be read/write by a client.
    mb_mapping_ = modbus_mapping_new(MODBUS_MAX_READ_BITS, 0,
                                     MODBUS_MAX_READ_REGISTERS, 0);
    if (!mb_mapping_) {
      throw std::runtime_error("modbus_mapping_new error");
    }
  }

  int SetServer(int server) override final {
    return modbus_set_slave(ctx_, server);
  }

  void Start() override final {
    int rc;

    // Will contain internal libmodubs data from a request that
    // must be given back to answer
    uint8_t *request = (uint8_t *)malloc(MODBUS_RTU_MAX_ADU_LENGTH);

    while (!exit_condition) {
      do {
        rc = modbus_receive(ctx_, request);
      } while (rc == 0 && !exit_condition);
      if (exit_condition) {
        return;
      }

      std::cout << "\nProcessing frame..." << std::endl;
      if (rc < 0) {
        std::cerr << "Error in modbus receive" << std::endl;
      }
      std::cout << "Request received of length " << rc << " bytes" << std::endl;
      if (modbus_reply(ctx_, request, rc, mb_mapping_) < 0) {
        throw std::runtime_error("modbus_reply error");
      }
    }
  }

  void Stop() override final { exit_condition = true; }

private:
  modbus_t *ctx_;
  modbus_mapping_t *mb_mapping_;
  const Config config_;
};
} // namespace

std::unique_ptr<Server> Server::Create(const Config &config) {
  auto server = std::make_unique<ServerImpl>(config);
  server->Connect();
  return server;
}
} // namespace vfdrtu
