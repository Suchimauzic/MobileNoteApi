cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(MobileNoteApi)

set(MOBILE_UNIT_TEST TRUE)

if (MOBILE_UNIT_TEST)
    cmake_policy(SET CMP0079 NEW)
    add_subdirectory(lib/googletest)
    add_subdirectory(tests)
endif()

# Skip pistache tests
option(PISTACHE_BUILD_TESTS "build tests alongside the project" OFF)
# Skip pqxx tests
set(SKIP_BUILD_TEST ON)

# Add libs
add_subdirectory(lib/libpqxx)
add_subdirectory(lib/pistache)
add_subdirectory(lib/json)

# Project
file(GLOB CPPS src/*.cpp)
include_directories(include/)
add_executable(${PROJECT_NAME} ${CPPS})

target_link_libraries(${PROJECT_NAME} PUBLIC libpqxx::pqxx pistache nlohmann_json::nlohmann_json)

if (MOBILE_UNIT_TEST)
    target_link_libraries(tests PUBLIC gtest gtest_main)
endif()