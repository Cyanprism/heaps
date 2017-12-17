cmake_minimum_required(VERSION 3.8)
project(GoogleTest)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(googletest-master)

include_directories(googletest-master/googletest/include)
set(SOURCE_FILES main.cpp IHeap.h BinomialHeap.h LeftistHeap.h SkewHeap.h STLHeap.h)
add_executable(GoogleTest ${SOURCE_FILES})
target_link_libraries(GoogleTest gtest gtest_main)