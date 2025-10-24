cmake_minimum_required(VERSION 3.10)
project(Atelier2)

set(CMAKE_CXX_STANDARD 20)

# --- OpenSSL paths ---
set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libcrypto.lib")
set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libssl.lib")

# --- Add executable ---
add_executable(atelier2_part1 main.cpp)

# --- Include OpenSSL headers ---
target_include_directories(atelier2_part1 PRIVATE "${OPENSSL_INCLUDE_DIR}")

# --- Link OpenSSL libraries ---
target_link_directories(atelier2_part1 PRIVATE "${OPENSSL_ROOT_DIR}/lib")
target_link_libraries(atelier2_part1 PRIVATE libcrypto libssl)
