cmake_minimum_required(VERSION 2.8.9)
project (chouinealgo)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_library(chouinealgo STATIC ${SOURCES})

add_executable(chouinetest test/ChouineTest.cpp)
target_link_libraries(chouinetest chouinealgo )