# Copyright (c) 2024-present, Ian Dinwoodie.
# Distributed under the MIT License (http://opensource.org/licenses/MIT).

message(STATUS "Finding libmodbus")

find_package(PkgConfig REQUIRED)
pkg_check_modules(libmodbus_pkg QUIET libmodbus)

if (NOT libmodbus_pkg_FOUND)
  set(libmodbus_version "3.1.11")
  message(STATUS "Fetching libmodbus source ${libmodbus_version}")

  # Use FetchContent to download libmodbus source so that we get the
  # FetchContent-style directory structure.
  include(FetchContent)
  FetchContent_Declare(libmodbus_src
    GIT_REPOSITORY "https://github.com/stephane/libmodbus.git"
    GIT_TAG "v${libmodbus_version}"
  )
  FetchContent_MakeAvailable(libmodbus_src)

  include(ExternalProject)
  ExternalProject_Add(libmodbus_project
    SOURCE_DIR ${libmodbus_src_SOURCE_DIR}
    DOWNLOAD_COMMAND cd ${libmodbus_src_SOURCE_DIR} && ./autogen.sh
    CONFIGURE_COMMAND ${libmodbus_src_SOURCE_DIR}/configure
      --srcdir=${libmodbus_src_SOURCE_DIR} --prefix=${libmodbus_src_BINARY_DIR}
      --enable-static=yes --disable-shared
    BUILD_COMMAND make -j ${CMAKE_BUILD_PARALLEL_LEVEL}
    INSTALL_COMMAND make install
  )

  set(LIBMODBUS_INCLUDE_DIRS ${libmodbus_src_BINARY_DIR}/include/modbus)
  file(MAKE_DIRECTORY ${LIBMODBUS_INCLUDE_DIRS})
  set(LIBMODBUS_LINK_LIBRARIES ${libmodbus_src_BINARY_DIR}/lib/libmodbus.a)
  set(MODBUS_CFLAGS_OTHER "")
else ()
  set(LIBMODBUS_INCLUDE_DIRS ${libmodbus_pkg_INCLUDE_DIRS})
  set(LIBMODBUS_LINK_LIBRARIES ${libmodbus_pkg_LINK_LIBRARIES})
  set(LIBMODBUS_CFLAGS_OTHER ${libmodbus_pkg_CFLAGS_OTHER})
endif ()

add_library(libmodbus STATIC IMPORTED GLOBAL)
add_dependencies(libmodbus libmodbus_project)
set_target_properties(libmodbus PROPERTIES IMPORTED_LOCATION ${LIBMODBUS_LINK_LIBRARIES})
set_target_properties(libmodbus PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${LIBMODBUS_INCLUDE_DIRS})
