cmake_minimum_required(VERSION 3.30)
project(CppLambda)

set(CMAKE_CXX_STANDARD 20)

add_executable(CppLambda main.cpp test_cases.cpp)
target_link_libraries(CppLambda PRIVATE Catch2::Catch2WithMain)
