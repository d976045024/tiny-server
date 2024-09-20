cmake_minimum_required(VERSION 2.6)

project(tiny-server C CXX)

enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

include_directories(${PROJECT_SOURCE_DIR})
