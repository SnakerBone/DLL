cmake_minimum_required(VERSION 3.26)
project(JNI)

find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)

add_executable(JNI main.cpp
        snkr.cpp)
