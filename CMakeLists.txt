project(Flappy)
cmake_minimum_required(VERSION 2.8)

#build type
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")

#flags
add_definitions(-std=c++14 -Wall -Wextra -m64)

#libs
include_directories(SYSTEM D:/libs/ECS/include)
link_directories(D:/libs/ECS/lib)
include_directories(SYSTEM D:/libs/SFML/include)
link_directories(D:/libs/SFML/lib)
include_directories(SYSTEM D:/libs/boost)

#main dir
include_directories(src)

#project files and include pathes
file(GLOB_RECURSE SRCS "src/*.cpp")

#build game
add_executable(flappy ${SRCS})
target_link_libraries(flappy sfml-system-d sfml-window-d sfml-graphics-d ecsLib)
