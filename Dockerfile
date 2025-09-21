FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    nasm \
    gcc \
    make \
    qemu-system-x86 \
    qemu-utils \
    binutils \
    kmod \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

RUN useradd -m -s /bin/bash builder && \
    chown -R builder:builder /workspace


RUN chmod +s /usr/bin/qemu-system-i386

USER builder

CMD ["bash"]
