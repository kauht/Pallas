cmake_minimum_required(VERSION 3.10)
project(pallas)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER_ID "GNU")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_FILES src/*.c)
add_executable(pallas ${SRC_FILES})

target_include_directories(pallas PRIVATE include)
target_compile_options(pallas PRIVATE -Wall -Wextra -g)
