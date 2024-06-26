set(headers 
    "src/lux_sp/assertions.h"
    "src/lux_sp/concrete_no_return.h"
    "src/lux_sp/concrete_system.h"
    "src/lux_sp/concurrency_support.h"
    "src/lux_sp/memory_pool.h"
    "src/lux_sp/no_return.h"
    "src/lux_sp/spsc/lock_free_queue.h"
    "src/lux_sp/system.h"
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
    "test/unit/lux_sp/spsc/test_lock_free_queue.cc"
    "test/unit/lux_sp/test_assertions.cc"
    "test/unit/lux_sp/test_concurrency_support.cc"
    "test/unit/lux_sp/test_memory_pool.cc"
)
add_executable(lux_sp_tests ${test_sources})
target_link_libraries(lux_sp_tests 
    PRIVATE
    lux_sp
    GTest::gmock_main
)
target_include_directories(lux_sp_tests
    PRIVATE
    "test/unit"
)
target_set_default_settings(lux_sp_tests)
gtest_discover_tests(lux_sp_tests)
