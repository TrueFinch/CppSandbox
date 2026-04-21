cmake_minimum_required(VERSION 3.30)
project(TypePrinter)

find_package(Catch2 REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(TypePrinter main.cpp type_name.h)
add_executable(TypePrinterTests type_name.h test_cases.cpp)
target_link_libraries(TypePrinterTests PRIVATE Catch2::Catch2WithMain)
