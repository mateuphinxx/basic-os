# Setup Environment

## Requirements
- NASM assembler
- GCC compiler
- Make
- QEMU (for testing)

## Windows Setup

### WSL (Recommended)
```bash
wsl --install -d Ubuntu
sudo apt update
sudo apt install nasm gcc make qemu-system-x86
```

### Docker
```bash
docker build -t basicos-dev .
docker run -v ${PWD}:/workspace basicos-dev make all
```

### Manual Install
1. Download NASM from https://www.nasm.us
2. Install MinGW-w64 for GCC
3. Add to PATH

## Build
```bash
make all
make run
```

Or use the build script:
```cmd
build.bat
```
