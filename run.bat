@echo off
echo Running BasicOS...

if not exist build_output\basicos.bin (
    echo Error: BasicOS binary not found!
    echo Please build first using: build.bat
    echo.
    pause
    exit /b 1
)

REM Check if QEMU is available
qemu-system-x86_64 --version >nul 2>&1
if %errorlevel% == 0 (
    echo Starting BasicOS with QEMU...
    taskkill /f /im qemu-system-x86_64.exe >nul 2>&1
    qemu-system-x86_64 -drive file=build_output\basicos.bin,format=raw,if=floppy -boot a
    goto :end
)

REM Try with Docker QEMU
docker --version >nul 2>&1
if %errorlevel% == 0 (
    echo Starting BasicOS with Docker QEMU...
    docker run --rm -it -v %CD%:/workspace basicos-dev qemu-system-x86_64 -fda /workspace/build_output/basicos.bin -boot a -nographic
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
