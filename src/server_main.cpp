// Local Headers
#include "config.h"
#include "server.h"

// C++ Standard Library Headers
#include <functional>
#include <iostream>
#include <thread>

// C Standard Library Headers
#include <unistd.h>

namespace {
void PrintUsage() {
  std::cout << "Usage: client [-b <baudrate>] -p <port>\n"
            << "       client [-h]\n"
            << "  -b baudrate\tSet the baud rate (default: 9600)\n"
            << "  -p port\tSet the device port\n"
            << "  -h\t\tPrint this help" << std::endl;
}

// Function to parse the command line arguments
vfdrtu::Config ParseArgs(int argc, char *argv[]) {
  vfdrtu::Config config;

  int opt;
  while ((opt = getopt(argc, argv, "b:p:h")) != -1) {
    switch (opt) {
    case 'b':
      config.baud = atoi(optarg);
      break;
    case 'p':
      config.port = std::string(optarg ? optarg : "");
      break;
    case 'h':
      PrintUsage();
      exit(0);
    default:
      PrintUsage();
      exit(1);
    }
  }

  if (config.port.empty()) {
    std::cerr << "Error: port is required" << std::endl;
    PrintUsage();
    exit(1);
  }

  return config;
}

void PrintBanner() {
  std::string banner = R"(  _   _________  ___  ________  __
 | | / / __/ _ \/ _ \/_  __/ / / /
 | |/ / _// // / , _/ / / / /_/ /
 |___/_/ /____/_/|_| /_/  \____/
  / __/__ _____  _____ ____
 _\ \/ -_) __/ |/ / -_) __/
/___/\__/_/  |___/\__/_/
)";
  std::cout << banner << std::endl;
}
} // namespace

int main(int argc, char *argv[]) {
  auto config = ParseArgs(argc, argv);

  PrintBanner();
  vfdrtu::Config::Print(config);

  try {
    auto server = vfdrtu::Server::Create(config);

    std::cout << "Entering server loop. Press Ctrl+C to exit.\n" << std::endl;
    auto exec_fn = std::bind(&vfdrtu::Server::Start, server.get());
    std::thread loop_thread = std::thread(exec_fn);

    // Wait for user input (single character). This is OS dependent.
#ifdef _WIN32
    system("pause");
#else
    system("read -n1");
#endif
    // Set the flag with true to break the loop.
    server->Stop();
    // Wait for the thread to finish.
    loop_thread.join();

    return 0;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
