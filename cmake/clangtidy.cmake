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

