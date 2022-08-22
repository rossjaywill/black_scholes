if (CPPCHECK)
    find_program(
        CPPCHECK_EXE
        NAMES cppcheck
    )
    if (NOT CPPCHECK_EXE)
        message(STATUS "Cppcheck enabled, but bin not found. Skipping")
    else()
        message(STATUS "Cppcheck tidy enabled")
        set(CMAKE_CXX_CPPCHECK cppcheck)
    endif()
else()
    message(STATUS "Cppcheck not enabled")
endif()

