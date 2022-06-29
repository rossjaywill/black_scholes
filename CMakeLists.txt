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

if (CLANG_TIDY)
    find_program(
        CLANG_TIDY_EXE
        NAMES clang-tidy clang-tidy-11 clang-tidy14
    )
    if (NOT CLANG_TIDY_EXE)
        message(STATUS "Clang tidy enabled, but bin not found. Skipping")
    else()
        message(STATUS "Clang tidy enabled")
        set(CMAKE_CXX_CLANG_TIDY clang-tidy)
    endif()
else()
    message(STATUS "Clang tidy not enabled")
endif()

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

target_link_libraries(bsm ${CONAN_LIBS})
target_compile_features(bsm PUBLIC cxx_std_20)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
