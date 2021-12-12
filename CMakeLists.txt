cmake_minimum_required(VERSION 3.15)
project(aoc_2021)

set(CMAKE_CXX_STANDARD 20)

set(COMMON_SOURCES Utils.cpp)

add_executable(aoc_2021_day_1 day_1.cpp ${COMMON_SOURCES})
add_executable(aoc_2021_day_2 day_2.cpp ${COMMON_SOURCES})
add_executable(aoc_2021_day_3 day_3.cpp ${COMMON_SOURCES})
add_executable(aoc_2021_day_4 day_4.cpp ${COMMON_SOURCES})
add_executable(aoc_2021_day_5 day_5.cpp ${COMMON_SOURCES})
add_executable(aoc_2021_day_6 day_6.cpp ${COMMON_SOURCES})
add_executable(aoc_2021_day_7 day_7.cpp ${COMMON_SOURCES})
add_executable(aoc_2021_day_8 day_8.cpp ${COMMON_SOURCES})
add_executable(aoc_2021_day_9 day_9.cpp ${COMMON_SOURCES})
add_executable(aoc_2021_day_10 day_10.cpp ${COMMON_SOURCES})
add_executable(aoc_2021_day_11 day_11.cpp ${COMMON_SOURCES})
add_executable(aoc_2021_day_12 day_12.cpp ${COMMON_SOURCES})
