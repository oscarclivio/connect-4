set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
cmake_minimum_required(VERSION 2.6)
file(TO_CMAKE_PATH "$ENV{IMAGINEPP_ROOT}/CMake" p)
list(APPEND CMAKE_MODULE_PATH "${p}") #For old Imagine++
list(APPEND CMAKE_SYSTEM_FRAMEWORK_PATH /Library/Frameworks)
find_package(Imagine REQUIRED)

project(Connect4)
add_executable(Connect4 Connect4.cpp Grid.cpp Grid.h PlayerGame.cpp PlayerGame.h Genome.cpp Genome.h)
ImagineUseModules(Connect4 Graphics)
