cmake_minimum_required(VERSION 3.10)
project(oop_lab6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

add_executable(${CMAKE_PROJECT_NAME}_exe main.cpp NPC.cpp dragon.cpp princess.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}_exe)