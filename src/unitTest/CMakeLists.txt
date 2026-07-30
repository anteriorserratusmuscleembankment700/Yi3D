cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "unitTest")
set(EXE_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

include(gtest)

find_package(wydb REQUIRED)

file(GLOB_RECURSE SRCS ./*.cpp)
add_executable(${EXE_NAME} ${SRCS})

target_include_directories(${EXE_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/inc"
    "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(${EXE_NAME} PRIVATE wydb wy3d gtest)
