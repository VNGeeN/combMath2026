@echo off
set BIN_DIR=%~dp0

echo Running unit tests...
"%BIN_DIR%salesman_tests.exe"
if errorlevel 1 exit /b 1

echo Running app on matrix4...
"%BIN_DIR%salesman.exe" "%BIN_DIR%data\matrix4.txt" "%BIN_DIR%out_matrix4.txt"
if errorlevel 1 exit /b 1

echo OK