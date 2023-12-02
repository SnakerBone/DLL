cmake_minimum_required(VERSION 3.26)
project(DLL)

find_package(JNI REQUIRED)

include_directories(${JNI_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES
        main.cpp
        snkr/snkr.cpp
        logger/logger.cpp
        logger/logger.h
)

add_executable(DLL ${SOURCE_FILES})

file(GLOB SOURCES *.cpp)