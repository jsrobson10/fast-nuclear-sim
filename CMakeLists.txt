cmake_minimum_required(VERSION 3.25)

project(FastNuclearSim VERSION 1.0)

set(CMAKE_CXX_STANDARD 26)

if(WIN32)
	set(CMAKE_CXX_FLAGS "-g -O3")
else()
	set(CMAKE_CXX_FLAGS "-g -O3 -I/usr/include/freetype2")
endif()

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(FastNuclearSim ${SOURCES})

if(WIN32)
	target_link_libraries(FastNuclearSim PUBLIC stdc++ m brotlidec assimp-5 glew32 opengl32 glfw3 freetype jsoncpp zlibstatic)
else()
	target_link_libraries(FastNuclearSim PUBLIC stdc++ m GLEW glfw GL freetype assimp jsoncpp)
endif()

