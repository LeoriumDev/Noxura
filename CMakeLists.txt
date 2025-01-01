cmake_minimum_required(VERSION 3.30)
project(Noxura)

set(CMAKE_CXX_STANDARD 23)

add_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

add_executable(Noxura src/main.cpp
        src/utils/Dependencies.hpp
        src/utils/Definitions.hpp
        src/delegates/AppDelegate.cpp
        src/delegates/AppDelegate.hpp
        src/delegates/ViewDelegate.cpp
        src/delegates/ViewDelegate.hpp
        src/renderer/Renderer.cpp
        src/renderer/Renderer.hpp)

target_include_directories(Noxura PRIVATE
        include/graphics/metal-cpp
        include/graphics/metal-cpp-extensions
        ${PROJECT_SOURCE_DIR}/src/)

target_link_libraries(Noxura PUBLIC
        "-framework Metal"
        "-framework MetalKit"
        "-framework AppKit"
        "-framework Foundation"
        "-framework QuartzCore"
)