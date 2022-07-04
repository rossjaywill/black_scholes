project(black_scholes_tests LANGUAGES CXX)

add_executable(
    bsm_tests
    main.cpp
    tst_black_scholes.cpp
    tst_greeks.cpp
    tst_input.cpp
)

target_include_directories(
    bsm_tests
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/tst
)

target_sources(
    bsm_tests
    PRIVATE
    ${CMAKE_SOURCE_DIR}/bsm/arg_parser.cpp
)
