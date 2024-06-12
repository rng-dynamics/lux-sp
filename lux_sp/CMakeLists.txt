set(headers 
    "src/lux_sp/predicates.h"
    "src/lux_sp/memory_pool.h"
    "src/lux_sp/system.h"
    "src/lux_sp/thread_support.h"
)

## Library
add_library(lux_sp OBJECT)
target_set_default_settings(lux_sp)
set_target_properties(lux_sp PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(lux_sp
    PUBLIC
    fmt
)
target_sources(lux_sp
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_LIST_DIR}/src
    FILES ${headers}
)
install(TARGETS lux_sp FILE_SET HEADERS)


## Unit tests
include(GoogleTest)
set(test_sources
    "src/lux_sp/predicates.test.cc"
    "src/lux_sp/memory_pool.test.cc"
    "src/lux_sp/thread_support.test.cc"
)
add_executable(lux_sp_tests ${test_sources})
target_link_libraries(lux_sp_tests 
    PRIVATE
    lux_sp
    GTest::gmock_main
)
target_set_default_settings(lux_sp_tests)
gtest_discover_tests(lux_sp_tests)
