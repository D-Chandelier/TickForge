cmake_minimum_required(VERSION 3.20)
project(TickForge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# -------------------
# nlohmann/json (header-only)
# -------------------
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# -------------------
# SDL3
# -------------------
set(SDL_SHARED OFF)
set(SDL_TEST_LIBRARY OFF)
set(SDL_TEST OFF)
set(SDL_STATIC ON)
set(SDL_DISABLE_INSTALL_DOCS OFF)
# Forcer le backend Windows / Direct3D
set(SDL_DIRECTX ON)
set(SDL_RENDER_D3D ON)
set(SDL_RENDER_D3D11 ON)
set(SDL_RENDER_D3D12 ON)
set(SDL_VULKAN ON)
set(SDL_OPENGL ON)
set(SDL_VIDEO_DRIVER_WINDOWS ON)
set(SDL_VIDEO_DRIVER_DUMMY OFF)
set(SDL_DUMMYVIDEO OFF)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.4.0
)

FetchContent_MakeAvailable(SDL3)

# build SDL3
# add_subdirectory(${SDL3_SOURCE_DIR} ${CMAKE_BINARY_DIR}/SDL3_build EXCLUDE_FROM_ALL)

# -------------------
# Executable
# -------------------
add_executable(tickforge
    src/main.cpp
    src/core/TickClock.cpp
    src/core/World.cpp
    src/core/Renderer.cpp
    src/core/Entity.cpp
    src/rules/RuleSystem.cpp
    src/rules/RuleLoader.cpp
)

target_include_directories(tickforge PRIVATE src)
target_include_directories(tickforge
    PRIVATE ${CMAKE_BINARY_DIR}/_deps/sdl3-src/include
)


target_link_libraries(tickforge PRIVATE 
    nlohmann_json::nlohmann_json
    SDL3::SDL3-static
    winmm  # timing / audio
    imm32  # input
    version
    setupapi
    user32
    gdi32
    ole32
    shell32
)

# -------------------
# Compiler warnings
# -------------------
if (MSVC)
    target_compile_options(tickforge PRIVATE /W4)
else()
    target_compile_options(tickforge PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Définit le working directory pour l'exécutable (Visual Studio)
set_target_properties(tickforge PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${RUNTIME_DIR}"
)