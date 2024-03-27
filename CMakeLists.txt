cmake_minimum_required(VERSION 3.25)

project(FastNuclearSim VERSION 1.0)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS "-g")

if(NOT DEBUG_SET)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()
if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/freetype2")
endif()

message("Using cmake flags: ${CMAKE_CXX_FLAGS}")
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(FastNuclearSim ${SOURCES})

if(WIN32)
	set(libs stdc++ m brotlidec assimp-5 glew32 opengl32 glfw3 freetype jsoncpp zlibstatic OpenAL32 vorbisfile alut)
else()
	set(libs stdc++ m GLEW glfw GL freetype assimp jsoncpp openal vorbisfile alut)
endif()

message("Using libs ${libs}")
target_link_libraries(FastNuclearSim PUBLIC ${libs})

