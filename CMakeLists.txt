cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(MobileNoteApi)

option(SKIP_BUILD_TEST OFF)

# Add libs
add_subdirectory(lib/libpqxx)
add_subdirectory(lib/pistache)
add_subdirectory(lib/json)

# Project
file(GLOB CPPS src/*.cpp)
include_directories(include/)
add_executable(${PROJECT_NAME} ${CPPS})

target_link_libraries(${PROJECT_NAME} PUBLIC libpqxx::pqxx pistache nlohmann_json::nlohmann_json)