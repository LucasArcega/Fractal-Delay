@echo off
setlocal
cd /d "%~dp0.."

call "%~dp0setup-vs-ninja.bat"
if errorlevel 1 exit /b 1

if exist "C:\Program Files\CMake\bin\cmake.exe" (
  "C:\Program Files\CMake\bin\cmake.exe" -B build -S . -G Ninja -DCMAKE_BUILD_TYPE=Debug
) else (
  cmake -B build -S . -G Ninja -DCMAKE_BUILD_TYPE=Debug
)
exit /b %ERRORLEVEL%
