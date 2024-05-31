#
# CMake project settings
#

# set default build type if not set
if (CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()
message(STATUS "CMAKE_BUILD_TYPE = [${CMAKE_BUILD_TYPE}]")

# set position independent code if defined
if (NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "fPIC default value" FORCE)
endif()
message(STATUS "CMAKE_POSITION_INDEPENDENT_CODE: [${CMAKE_POSITION_INDEPENDENT_CODE}]")

# create compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Create compile commands" FORCE)
message(STATUS "CMAKE_EXPORT_COMPILE_COMMANDS = [${CMAKE_EXPORT_COMPILE_COMMANDS}]")

# additional compile options
if (NOT DEFINED TARGET_DEFAULT_COMPILE_OPTIONS)
    set(TARGET_DEFAULT_COMPILE_OPTIONS "" CACHE STRING "Additional compile options default value" FORCE)
endif()
message(STATUS "TARGET_DEFAULT_COMPILE_OPTIONS: [${TARGET_DEFAULT_COMPILE_OPTIONS}]")

# additional link libraries
if (NOT DEFINED TARGET_DEFAULT_LINK_LIBRARIES)
    set(TARGET_DEFAULT_LINK_LIBRARIES "" CACHE STRING "Additional link libraries default value" FORCE)
endif()
message(STATUS "TARGET_DEFAULT_LINK_LIBRARIES: [${TARGET_DEFAULT_LINK_LIBRARIES}]")

# always enable cmake testing support
enable_testing()
