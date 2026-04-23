cmake_minimum_required(VERSION 3.30)
project(MyAny)

find_package(Catch2 REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(MyAny main.cpp Any.h)
add_executable(MyAnyTests Any.h test_cases.cpp)
target_link_libraries(MyAnyTests PRIVATE Catch2::Catch2WithMain)
