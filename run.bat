@echo off
echo Running BasicOS...

if not exist build\basicos.bin (
    echo Error: BasicOS binary not found!
    echo Please build first using: build.bat
    echo.
    pause
    exit /b 1
)

REM Check if QEMU is available
qemu-system-i386 --version >nul 2>&1
if %errorlevel% == 0 (
    echo Starting BasicOS with QEMU...
    qemu-system-i386 -fda build\basicos.bin
    goto :end
)

REM Try with Docker QEMU
docker --version >nul 2>&1
if %errorlevel% == 0 (
    echo Starting BasicOS with Docker QEMU...
    docker run --rm -v %CD%:/workspace basicos-dev qemu-system-i386 -fda build/basicos.bin -nographic
    goto :end
)

echo Error: QEMU not found!
echo.
echo Please install QEMU or use VirtualBox:
echo 1. Download QEMU from: https://www.qemu.org/download/
echo 2. Or use VirtualBox with build\basicos.bin as floppy disk
echo.

:end
pause
