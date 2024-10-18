# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(Test)

# C++20
set(CMAKE_CXX_STANDARD 20)

add_executable(jthread_test main.cpp)

target_link_libraries(jthread_test pthread)
