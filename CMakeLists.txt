# Copyright (c) 2024-present, Ian Dinwoodie.
# Distributed under the MIT License (http://opensource.org/licenses/MIT).

cmake_minimum_required(VERSION 3.14)

################################################################################
# CMake policies.

cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0135 NEW)

################################################################################
# Project settings.

project(vfdrtu VERSION 0.1.0 LANGUAGES CXX C)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Debug"
      CACHE STRING "Choose Release or Debug" FORCE)
endif ()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Use standardized build/install paths.
include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

################################################################################
# Compiler settings.

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

################################################################################
# Third party libraries.

include("cmake/FindLibmodbus.cmake")

################################################################################
# Executables.

add_executable(server
  src/server_main.cpp
  src/config.cpp
  src/server.cpp
)

target_link_libraries(server PUBLIC libmodbus)

add_executable(client
  src/client_main.cpp
  src/client.cpp
  src/config.cpp
)

target_link_libraries(client PUBLIC libmodbus)

################################################################################
