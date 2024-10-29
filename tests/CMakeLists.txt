cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(tests)

include_directories(include/)
add_executable(${PROJECT_NAME} tests.cpp ${CMAKE_SOURCE_DIR}/src/ManageDB.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/)
