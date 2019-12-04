cmake_minimum_required(VERSION 3.12)
project(RNS_MODULI_GENERATOR)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

find_package(MPFR REQUIRED)
if (NOT ${MPFR_FOUND})
    message(SEND_ERROR "Failed to find MPFR")
    return()
else ()
    include_directories(${MPFR_INCLUDES})
endif ()

add_executable(RNS_MODULI_GENERATOR main.cpp)

target_link_libraries(RNS_MODULI_GENERATOR ${MPFR_LIBRARY})