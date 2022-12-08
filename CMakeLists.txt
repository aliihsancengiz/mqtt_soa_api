cmake_minimum_required(VERSION 3.15)

project(mqtt-ipc VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TEST "Build with test" OFF)

find_package(PahoMqttCpp)
add_subdirectory(src)

if(BUILD_TEST)
    add_subdirectory(test)
endif(BUILD_TEST)
