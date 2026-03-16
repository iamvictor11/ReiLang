@echo off
cd /d %~dp0
if not exist build_release mkdir build_release
cmake -S . -B build_release -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build_release
pause
