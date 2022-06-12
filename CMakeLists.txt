cmake_minimum_required(VERSION 3.22.1)

project(black_scholes_cpp)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(tst)

add_executable(main src/main.cpp)
target_include_directories(main PRIVATE include)
target_link_libraries(main ${CONAN_LIBS})
target_compile_features(main PUBLIC cxx_std_20)
