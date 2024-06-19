#
# functions
#

# set default settings for a target
function(TARGET_SET_DEFAULT_SETTINGS TARGET)
    if(ARGN)
        message(FATAL_ERROR "Invalid arguments [${ARGN}]")
    endif()

    target_compile_features("${TARGET}" PRIVATE cxx_std_20)
    set_target_properties("${TARGET}" PROPERTIES
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # enable compiler warnings
        target_compile_options("${TARGET}" PRIVATE "-Wall" "-Wextra" "-pedantic")
        # warnings are errors
        target_compile_options("${TARGET}" PRIVATE "-Werror" "-pedantic-errors")
        # add addtional compile options from user
        STRING(REPLACE " " ";" TARGET_DEFAULT_COMPILE_OPTIONS "${TARGET_DEFAULT_COMPILE_OPTIONS}")
        target_compile_options("${TARGET}" PRIVATE "${TARGET_DEFAULT_COMPILE_OPTIONS}")
        # add addtional link libraries from user
        STRING(REPLACE " " ";" TARGET_DEFAULT_LINK_LIBRARIES "${TARGET_DEFAULT_LINK_LIBRARIES}")
        target_link_libraries("${TARGET}" PRIVATE "${TARGET_DEFAULT_LINK_LIBRARIES}")
        # optionally enable code coverage
        if(ENABLE_CODECOVERAGE)
            target_compile_options("${TARGET}" PRIVATE "--coverage")
            target_link_options("${TARGET}" PRIVATE "--coverage")
            target_compile_definitions("${TARGET}" PUBLIC ENABLE_GCOV)
        endif()
    else()
        message(FATAL_ERROR "Compiler with ID '${CMAKE_CXX_COMPILER_ID}' not supported")
    endif()
endfunction()

#
# code coverage
#

if (ENABLE_CODECOVERAGE)
    find_program(GCOV_COMMAND gcov REQUIRED)
    message(STATUS "GCOV_COMMAND = [${GCOV_COMMAND}]")
    find_program(GCOVR_COMMAND gcovr REQUIRED)
    message(STATUS "GCOVR_COMMAND = [${GCOVR_COMMAND}]")

    set(GCOVR_OUTPUT_DIR "${CMAKE_BINARY_DIR}/gcovr")
    message(STATUS "GCOVR_OUTPUT_DIR = [${GCOVR_OUTPUT_DIR}]")

    set(GCOVR_COMMAND_LINE
        "${GCOVR_COMMAND}"
        "--gcov-executable" "${GCOV_COMMAND}"
        "--root" "${PROJECT_SOURCE_DIR}"
        "--filter" "${PROJECT_SOURCE_DIR}/lux_sp"
        "--exclude" ".*/test/.*"  # exclude files in test folder from coverage
        "--exclude-unreachable-branches"
        # "--exclude-throw-branches"
        "--cobertura" "${GCOVR_OUTPUT_DIR}/cobertura.xml"
        "--xml-pretty"
        "--html-details" "${GCOVR_OUTPUT_DIR}/html/index.html"
        "--print-summary"
    )
    message(STATUS "GCOVR_COMMAND_LINE = [${GCOVR_COMMAND_LINE}]")

    add_custom_target(run-gcovr
        COMMAND ${CMAKE_COMMAND} "-E" "remove_directory" "${GCOVR_OUTPUT_DIR}"
        COMMAND ${CMAKE_COMMAND} "-E" "make_directory" "${GCOVR_OUTPUT_DIR}"
        COMMAND ${CMAKE_COMMAND} "-E" "make_directory" "${GCOVR_OUTPUT_DIR}/html"
        COMMAND ${GCOVR_COMMAND_LINE}
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Running gcovr"
    )
endif()
