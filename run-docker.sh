#!/bin/bash

echo "Building BasicOS..."
docker build -t basicos-dev .

echo "Compiling BasicOS..."
docker run --rm -v ${PWD}:/workspace basicos-dev make all

echo "Starting BasicOS in Docker..."
docker run --rm -it \
  -v ${PWD}:/workspace \
  basicos-dev \
  qemu-system-x86_64 -fda /workspace/build_output/basicos.bin -boot a -nographic
