project(black_scholes_tests LANGUAGES CXX)

add_executable(bsc_tests tst_input.cpp)
target_include_directories(bsc_tests PRIVATE include)
