set(sources 
    "src/lux_sp/dummy.cc"
    "src/lux_sp/threads.cc"
    "src/lux_sp/system_calls.cc"
)
set(private_headers_dir
    "private_include/"
)
set(public_headers_dir
    "public_include/"
)
set(public_headers
    "${public_headers_dir}/lux_sp/dummy.h"
    "${public_headers_dir}/lux_sp/threads.h"
)


## Library
add_library(lux_sp STATIC)
target_set_default_settings(lux_sp)
target_include_directories(lux_sp
    PRIVATE ${private_headers_dir}
    # PUBLIC ${public_headers_dir} # TODO: remove
)
target_sources(lux_sp
    PRIVATE ${sources}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_LIST_DIR}/${public_headers_dir}
    FILES ${public_headers}
)
target_link_libraries(lux_sp
    PUBLIC
    fmt # TODO: remove
)
install(TARGETS lux_sp FILE_SET HEADERS)


## Unit tests
include(GoogleTest)
set(test_sources
    "src/lux_sp/dummy.test.cc"
    "src/lux_sp/threads.test.cc"
)
add_executable(lux_sp_tests ${test_sources})
target_link_libraries(lux_sp_tests 
    PRIVATE
    lux_sp
    GTest::gmock_main
)
target_set_default_settings(lux_sp_tests)
gtest_discover_tests(lux_sp_tests)
