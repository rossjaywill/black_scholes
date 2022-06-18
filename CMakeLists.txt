cmake_minimum_required(VERSION 3.22.1)

project(black_scholes_cpp LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(tst)

add_executable(bsm bsm/main.cpp)
target_include_directories(bsm PRIVATE include)
target_link_libraries(bsm ${CONAN_LIBS})
target_compile_features(bsm PUBLIC cxx_std_20)
