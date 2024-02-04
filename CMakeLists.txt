cmake_minimum_required(VERSION 3.25)

project(FastNuclearSim VERSION 1.0)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS "-g -O3 -I/usr/include/freetype2")

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(FastNuclearSim ${SOURCES})
target_link_libraries(FastNuclearSim PUBLIC stdc++ m GLEW glfw GL freetype assimp)

