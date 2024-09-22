cmake_minimum_required(VERSION 3.10)

project(tiny-server C CXX)

enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wextra -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-compare -Wshadow -Wno-unused-parameter -Wno-unused-variable -Woverloaded-virtual -Wnon-virtual-dtor -Wno-missing-field-initializers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -D_GLIBCXX_USE_CXX11_ABI=0")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/unittest/)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/third-party/gtest-1.8.1/fused-src)
find_package(Threads REQUIRED)

add_subdirectory(third-party/gtest-1.8.1/fused-src/gtest)
add_subdirectory(base)
add_subdirectory(net)

set(TESTS 
      unittest/net/buffer_test.cc
      unittest/net/eventloop_test.cc
      unittest/base/current_thread_test.cc
      unittest/base/timestamp_test.cc)

foreach(sourcefile ${TESTS})
  get_filename_component(exename ${sourcefile} NAME_WE)
  add_executable(${exename}${ARTIFACT_SUFFIX} ${sourcefile})
  target_link_libraries(${exename}${ARTIFACT_SUFFIX}
    server_net gtest)
endforeach(sourcefile ${TESTS})
