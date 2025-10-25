cmake_minimum_required(VERSION 3.10)
project(Atelier2)

set(CMAKE_CXX_STANDARD 20)

# --- OpenSSL Configuration ---
set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
set(OPENSSL_BIN_DIR "${OPENSSL_ROOT_DIR}/bin")
set(OPENSSL_LIB_DIR "${OPENSSL_ROOT_DIR}/lib")

# Check if OpenSSL exists
if(NOT EXISTS "${OPENSSL_INCLUDE_DIR}/openssl/sha.h")
    message(FATAL_ERROR "OpenSSL not found at ${OPENSSL_ROOT_DIR}!\nPlease install from: https://slproweb.com/products/Win32OpenSSL.html")
endif()

# --- Add executable ---
add_executable(atelier2_part1 main.cpp)

# --- Include OpenSSL headers ---
target_include_directories(atelier2_part1 PRIVATE "${OPENSSL_INCLUDE_DIR}")

# --- Link OpenSSL libraries ---
# Different linking approach for MSVC vs MinGW
if(MSVC)
    # MSVC: Use .lib files from lib/VC directory
    target_link_libraries(atelier2_part1 PRIVATE 
        "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libcrypto.lib"
        "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libssl.lib"
        ws2_32
        crypt32
    )
    # Copy DLLs to output directory
    add_custom_command(TARGET atelier2_part1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OPENSSL_BIN_DIR}/libcrypto-3-x64.dll"
            "${OPENSSL_BIN_DIR}/libssl-3-x64.dll"
            $<TARGET_FILE_DIR:atelier2_part1>
        COMMENT "Copying OpenSSL DLLs to output directory"
    )
else()
    # MinGW: Try to link with library names (requires .a files or DLLs)
    target_link_directories(atelier2_part1 PRIVATE "${OPENSSL_BIN_DIR}")
    target_link_directories(atelier2_part1 PRIVATE "${OPENSSL_LIB_DIR}")
    
    target_link_libraries(atelier2_part1 PRIVATE 
        crypto-3-x64
        ssl-3-x64
        ws2_32
        crypt32
    )
    # Copy DLLs to build directory
    add_custom_command(TARGET atelier2_part1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OPENSSL_BIN_DIR}/libcrypto-3-x64.dll"
            "${OPENSSL_BIN_DIR}/libssl-3-x64.dll"
            $<TARGET_FILE_DIR:atelier2_part1>
        COMMENT "Copying OpenSSL DLLs to build directory"
    )
endif()

