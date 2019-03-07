cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
#include_directories(~/bin/arduino-1.6.9/hardware/arduino/avr/cores/arduino/inc)
link_directories(${CMAKE_SOURCE_DIR}/lib)
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests Drive_test.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
