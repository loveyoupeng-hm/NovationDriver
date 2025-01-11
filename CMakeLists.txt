cmake_minimum_required(VERSION 3.31)

project(NovationDriver)

set(CMAKE_CXX_STANDARD 23)

set(PROJECT_VERSION 0.1.0)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(CPM_SOURCE_CACHE ${LIB_DIR}/cpm)

include(cmake/cpm.cmake)

find_package(Threads REQUIRED)

CPMAddPackage(
    NAME JUCE
    GITHUB_REPOSITORY juce-framework/JUCE
    GIT_TAG 8.0.6
    VERSION 8.0.6
    SOURCE_DIR ${LIB_DIR}/juce
)

CPMAddPackage(
    NAME GOOGLE_TEST
    GITHUB_REPOSITORY google/googletest
    VERSION 1.15.2
    SOURCE_DIR ${LIB_DIR}/googletest
    OPTIONS
        "INSTALL_GTEST OFF"
        "gtest_force_shared_crt ON"

)

# add_compile_options(-Wall -Wextra -Wpedantic)
include_directories(includes)

add_subdirectory(Launchpad)

enable_testing()
add_subdirectory(Tests)
