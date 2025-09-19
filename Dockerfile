FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    nasm \
    gcc \
    make \
    qemu-system-x86 \
    binutils \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

RUN useradd -m -s /bin/bash builder && \
    chown -R builder:builder /workspace

USER builder

CMD ["bash"]
