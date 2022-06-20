project(black_scholes_tests LANGUAGES CXX)

add_executable(bsc_tests main.cpp tst_input.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_include_directories(bsc_tests PRIVATE include)
