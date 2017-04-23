cmake_minimum_required(VERSION 2.8)

project(proactor)
set(CMAKE_CXX_FLAGS "-g -std=c++11")
include_directories(${proactor_SOURCE_DIR}/include)

enable_testing()
add_subdirectory(tests)
