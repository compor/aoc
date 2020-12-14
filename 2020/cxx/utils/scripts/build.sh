#!/usr/bin/env bash

CXX=clang++ CC=clang cmake \
  -GNinja \
  -DCMAKE_CXX_FLAGS="-O3" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=On \
  "${1}"
