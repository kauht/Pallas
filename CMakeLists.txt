cmake_minimum_required(VERSION 3.10)
project(pallas VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.cc
    src/*.cxx
)

add_executable(palc ${SRC_FILES})

target_include_directories(palc PRIVATE include)

target_compile_options(palc PRIVATE
    $<$<CONFIG:Debug>:-g>
)

target_compile_options(palc PRIVATE
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)
