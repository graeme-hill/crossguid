cmake_minimum_required(VERSION 3.4.1)

set(LIB_NAME crossguidtest)
set(XG_DIR ${CMAKE_CURRENT_LIST_DIR}/../..)
set(XG_TEST_DIR ${XG_DIR}/test)

add_library(${LIB_NAME} SHARED src/main/cpp/jnitest.cpp ${XG_TEST_DIR}/Test.cpp)

target_include_directories(${LIB_NAME} PRIVATE
                          ${XG_DIR}
                          ${XG_TEST_DIR})

target_compile_definitions(${LIB_NAME} PRIVATE GUID_ANDROID)

set(XG_TESTS OFF CACHE BOOL "disable tests")
add_subdirectory(${XG_DIR} ${XG_DIR}/cmake_build)

target_link_libraries(${LIB_NAME} xg)