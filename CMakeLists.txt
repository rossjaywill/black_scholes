cmake_minimum_required(VERSION 3.22.1)

project(black_scholes_cpp LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++20 -Og -ggdb -Wall -Wextra -pedantic -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++20 -O3 -Wall -Wextra -pedantic -Werror")

include(cmake/clangtidy.cmake)
include(cmake/cppcheck.cmake)

add_subdirectory(tst)

add_executable(
    bsm
    bsm/main.cpp
)

target_sources(
    bsm
    PRIVATE
    "${CMAKE_SOURCE_DIR}/bsm/arg_parser.cpp"
)

target_include_directories(
    bsm
    PRIVATE
    bsm
    include
)

include(cmake/pch.cmake)

target_link_libraries(bsm ${CONAN_LIBS})
target_compile_features(bsm PUBLIC cxx_std_20)

include(cmake/asan.cmake)
