#!/usr/bin/env bash

# This builds coverage information, including HTML output

set -euxo pipefail

if [ $# -ge 1 ]; then
  EXTRA_OPTIONS=$1
else
  EXTRA_OPTIONS=""
fi

BUILD_DIR=build/build-coverage

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
pushd "$BUILD_DIR"

cmake                                       \
    -DCMAKE_BUILD_TYPE=Debug                \
    -DHIGHFIVE_EXAMPLES=OFF                 \
    -DHIGHFIVE_UNIT_TESTS=OFF               \
    -G "${CMAKE_GENERATOR:-Unix Makefiles}" \
    ${EXTRA_OPTIONS}                        \
    ../..

cmake --build . -j --target coverage

