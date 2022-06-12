cmake_minimum_required(VERSION 3.22.1)

project(black_scholes_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(tst)


add_executable(main src/main.cpp)
target_include_directories(main PRIVATE include)
target_link_libraries(main ${CONAN_LIBS})
