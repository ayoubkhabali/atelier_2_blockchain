@echo off
REM Create MinGW import libraries for OpenSSL DLLs

echo Creating MinGW-compatible import libraries...
echo.

cd /d "%~dp0build"

REM Create import library for libcrypto
echo Creating libcrypto.a...
gendef "C:\Program Files\OpenSSL-Win64\bin\libcrypto-3-x64.dll" 2>nul
if exist libcrypto-3-x64.def (
    dlltool -d libcrypto-3-x64.def -l libcrypto.a -D libcrypto-3-x64.dll
    echo   libcrypto.a created successfully
) else (
    echo   ERROR: gendef not found. Please install mingw-w64-tools
    echo.
    echo   Alternative: Use Visual Studio compiler instead
    echo   See MINGW_OPENSSL_FIX.md for instructions
    pause
    exit /b 1
)

REM Create import library for libssl
echo Creating libssl.a...
gendef "C:\Program Files\OpenSSL-Win64\bin\libssl-3-x64.dll" 2>nul
if exist libssl-3-x64.def (
    dlltool -d libssl-3-x64.def -l libssl.a -D libssl-3-x64.dll
    echo   libssl.a created successfully
) else (
    echo   ERROR: Could not create libssl.a
    pause
    exit /b 1
)

echo.
echo Import libraries created successfully!
echo Now run: cmake .. -G "MinGW Makefiles" 
echo Then run: cmake --build .
echo.
pause
