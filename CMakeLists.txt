cmake_minimum_required(VERSION 3.15)
project(aoc_2021)

set(CMAKE_CXX_STANDARD 20)

set(COMMON_SOURCES Utils.cpp)

add_executable(aoc_2021_day_1 day_1.cpp ${COMMON_SOURCES})
