cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")

project (chouinealgo)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

add_library(chouinealgo STATIC ${SOURCES})

add_executable(chouinetest test/ChouineTest.cpp)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
target_link_libraries(chouinetest chouinealgo )

add_executable(unittests test/unittests.cpp)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
target_link_libraries(unittests chouinealgo )