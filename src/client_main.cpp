// Local Headers
#include "client.h"
#include "config.h"

// C++ Standard Library Headers
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

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
 |___/_/_/____/_/|_|_/_/  \____/
 / ___/ (_)__ ___  / /
/ /__/ / / -_) _ \/ __/
\___/_/_/\__/_//_/\__/
)";
  std::cout << banner << std::endl;
}

struct Command {
  uint16_t fn_addr;
  int reg_addr;
  uint16_t value;
};

std::atomic_bool exit_condition(false);

void ProcessCommands(vfdrtu::Client &client,
                     const std::vector<Command> &commands) {
  int idx = -1;
  while (!exit_condition) {
    idx = (idx + 1) % commands.size();
    const auto &cmd = commands[idx];

    std::cout << "\nExecuting command..." << std::endl;
    if (cmd.fn_addr == 0x06) {
      int ret = client.WriteRegister(cmd.reg_addr, cmd.value);
      if (ret < 0) {
        std::cerr << "Error: WriteRegister failed" << std::endl;
      }
    } else {
      std::cerr << "Error: Unsupported function code" << std::endl;
    }

    sleep(1);
  }
}
} // namespace

int main(int argc, char *argv[]) {
  auto config = ParseArgs(argc, argv);

  PrintBanner();
  vfdrtu::Config::Print(config);

  try {
    auto client = vfdrtu::Client::Create(config);

    std::vector<Command> commands = {
        {0x06, 0x66, 0x01},
        {0x06, 0x66, 0x00},
        {0x06, 0x66, 0x02},
        {0x06, 0x66, 0x03},
    };

    std::cout << "\nPress any key to end the program..." << std::endl;
    std::thread loop_thread =
        std::thread(ProcessCommands, std::ref(*client), std::ref(commands));

    // Wait for user input (single character). This is OS dependent.
#ifdef _WIN32
    system("pause");
#else
    system("read -n1");
#endif
    // Set the flag with true to break the loop.
    exit_condition = true;
    // Wait for the thread to finish.
    loop_thread.join();

    return 0;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
