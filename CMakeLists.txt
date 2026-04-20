cmake_minimum_required(VERSION 3.30)
project(CppSandbox)

find_package(Catch2 CONFIG REQUIRED)

add_subdirectory(Lambda)
add_subdirectory(MyAny)
