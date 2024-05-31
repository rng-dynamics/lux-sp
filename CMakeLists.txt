cmake_minimum_required(VERSION  3.25)
project(
    lux_sp
    VERSION 0.0.0
    LANGUAGES CXX
)

include(cmake/settings.cmake)
include(cmake/dependencies.cmake)
include(cmake/utilities.cmake)

add_subdirectory(lux_sp)
