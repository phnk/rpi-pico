#!/bin/bash
set -e

if [ "$1" == "--clean" ]; then
    rm -rf build
fi
cmake -B build
make -C build -j$(nproc)

cp build/main.uf2 /media/$USER/RP2350/
