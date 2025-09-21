@echo off
echo Building BasicOS...

REM Check if running in WSL
wsl -e which nasm >nul 2>&1
if %errorlevel% == 0 (
    echo Using WSL environment
    wsl make all
    echo OS binary available at: build_output/basicos.bin
    goto :end
)

REM Try Docker if available
docker --version >nul 2>&1
if %errorlevel% == 0 (
    echo Using Docker environment
    echo Building Docker image...
    docker build -t basicos-dev .
    echo Building BasicOS...
    docker run --rm -v %CD%:/workspace basicos-dev make all
    echo.
    echo Build completed successfully!
    echo OS binary available at: build_output\basicos.bin
    echo.
    echo To run BasicOS:
    echo   - With QEMU: qemu-system-x86_64 -fda build_output\basicos.bin
    echo   - With VirtualBox: Use build_output\basicos.bin as floppy disk
    echo.
    goto :end
)

echo Error: Please install NASM and GCC, or use WSL/Docker
echo See setup.md for instructions

:end
pause
