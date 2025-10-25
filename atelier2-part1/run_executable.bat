@echo off
REM ========================================
REM Run Compiled Executable
REM ========================================
REM This script can be run from anywhere or double-clicked

echo ========================================
echo   BLOCKCHAIN CA-HASH - TEST RESULTS
echo ========================================
echo.

REM Get the directory where this script is located
set SCRIPT_DIR=%~dp0

REM Check if executable exists in build-vs\Release
if exist "%SCRIPT_DIR%build-vs\Release\atelier2_part1.exe" (
    echo Running tests from build-vs\Release...
    echo.
    cd /d "%SCRIPT_DIR%"
    call build-vs\Release\atelier2_part1.exe
    goto end
)

REM Check if executable exists in build
if exist "%SCRIPT_DIR%build\atelier2_part1.exe" (
    echo Running tests from build...
    echo.
    cd /d "%SCRIPT_DIR%"
    call build\atelier2_part1.exe
    goto end
)

REM If not found
echo ========================================
echo ERROR: Executable not found!
echo ========================================
echo.
echo Please build the project first:
echo.
echo Option 1 - Visual Studio:
echo   Open "x64 Native Tools Command Prompt for VS 2019"
echo   Run: run_tests_vs.bat
echo.
echo Option 2 - MinGW (if compatible):
echo   Run: run_tests.bat
echo.
echo ========================================
pause
exit /b 1

:end
echo.
echo ========================================
echo   Execution completed!
echo ========================================
echo.
pause
