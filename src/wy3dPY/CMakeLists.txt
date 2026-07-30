cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "wy3dpy")
set(LIBRARY_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

file(GLOB_RECURSE SRCS ./*.cpp)

if(WIN32)
    find_package(Python310Embed)
endif()
find_package(wydb REQUIRED)
find_package(wyap REQUIRED)

# 创建python扩展模块
add_library(${LIBRARY_NAME} MODULE ${SRCS})

target_include_directories(${LIBRARY_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/inc"
    "${CMAKE_SOURCE_DIR}/3rdParty"
)
if(UNIX)
    target_include_directories(${LIBRARY_NAME} PRIVATE ${Python3_INCLUDE_DIRS})
endif()

# Keep Python.h auto-linking consistent with the debug Python import library.
target_compile_definitions(${LIBRARY_NAME} PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<PLATFORM_ID:Windows>>:Py_DEBUG>
)

if(WIN32)
    set_target_properties(${LIBRARY_NAME} PROPERTIES
        OUTPUT_NAME "wy3d"
        DEBUG_POSTFIX "_d"
        PREFIX ""
        ARCHIVE_OUTPUT_NAME ${LIBRARY_NAME}
        SUFFIX ".pyd"
    )
else()
    set_target_properties(${LIBRARY_NAME} PROPERTIES
        OUTPUT_NAME "wy3d"
        DEBUG_POSTFIX ""
        PREFIX ""
        ARCHIVE_OUTPUT_NAME ${LIBRARY_NAME}
        SUFFIX ".so"
    )
endif()

target_link_libraries(${LIBRARY_NAME} PRIVATE pybind11::headers)
# 链接
if(WIN32)
    target_link_libraries(${LIBRARY_NAME} PRIVATE Python310Embed::Python)
endif()
target_link_libraries(${LIBRARY_NAME} PRIVATE wydb wyap)
target_link_libraries(${LIBRARY_NAME} PRIVATE wy3d)
