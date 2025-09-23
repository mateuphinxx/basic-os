FROM alpine:3.22

RUN apk add --no-cache \
    nasm \
    gcc \
    musl-dev \
    make \
    qemu-system-x86_64 \
    binutils \
    bash

WORKDIR /workspace

RUN adduser -D -s /bin/bash builder && \
    chown -R builder:builder /workspace

USER builder

CMD ["bash"]
