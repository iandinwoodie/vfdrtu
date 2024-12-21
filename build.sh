#!/usr/bin/env bash

function main {
  local script_dir
  script_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)
  cd "${script_dir}" || exit 1

  local cores
  cores=$(nproc --all)

  cmake -S . -B build
  cmake --build build -j "${cores}"
}

main
