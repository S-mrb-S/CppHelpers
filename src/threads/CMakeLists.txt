# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(JThreadTest)

# C++20
set(CMAKE_CXX_STANDARD 20)

add_executable(jthread_test jthread_test.cpp)

target_link_libraries(jthread_test pthread)
