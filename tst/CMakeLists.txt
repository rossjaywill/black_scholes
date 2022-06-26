project(black_scholes_tests LANGUAGES CXX)

add_executable(
    bsm_tests
    main.cpp
    tst_input.cpp
    tst_black_scholes.cpp
    tst_greeks.cpp
)

include_directories("${CMAKE_SOURCE_DIR}/include")

target_include_directories(
    bsm_tests
    PUBLIC
    include
)

target_sources(
    bsm_tests
    PRIVATE
    ${CMAKE_SOURCE_DIR}/bsm/arg_parser.cpp
)
