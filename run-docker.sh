#!/bin/bash

echo "Building BasicOS..."
docker build -t basicos-dev .

echo "Compiling BasicOS..."
docker run --rm -v ${PWD}:/workspace basicos-dev make all

echo "Starting BasicOS with Docker (similar to qemus/qemu setup)..."

# Method 1: With KVM acceleration (if available)
docker run --rm -it \
  --name basicos-run \
  --device=/dev/kvm \
  --device=/dev/net/tun \
  --cap-add=NET_ADMIN \
  --cap-add=SYS_ADMIN \
  -v ${PWD}:/workspace \
  basicos-dev \
  qemu-system-i386 -fda /workspace/build_output/basicos.bin \
    -boot a -nographic -no-reboot \
    -enable-kvm -M pc -cpu host 2>/dev/null

# Method 2: Fallback without KVM (if Method 1 fails)
if [ $? -ne 0 ]; then
    echo "KVM not available, trying without hardware acceleration..."
    docker run --rm -it \
      --name basicos-run-fallback \
      -v ${PWD}:/workspace \
      basicos-dev \
      qemu-system-i386 -fda /workspace/build_output/basicos.bin \
        -boot a -nographic -no-reboot -M pc
fi
