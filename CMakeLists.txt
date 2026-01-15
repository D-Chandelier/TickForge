cmake_minimum_required(VERSION 3.20)
project(TickForge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(tickforge
    src/main.cpp
    src/core/TickClock.cpp
    src/core/World.cpp
    src/core/Entity.cpp
)

target_include_directories(tickforge PRIVATE src)

if (MSVC)
    target_compile_options(tickforge PRIVATE /W4)
else()
    target_compile_options(tickforge PRIVATE -Wall -Wextra -Wpedantic)
endif()
