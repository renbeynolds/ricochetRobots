cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(test/testBoard test/testBoard.cpp)
target_link_libraries(test/testBoard ${GTEST_LIBRARIES} pthread)
