cmake_minimum_required(VERSION 3.5.1)

project(testProj)

configure_file(CMakeLists.txt.in
               external/download/crossguid/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/download/crossguid )
  
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/download/crossguid )


find_package(xg CONFIG PATHS ${CMAKE_BINARY_DIR}/external/crossguid/xg)


# Find source files
file(GLOB TESTPROJ_SRC
    "*.h"
    "*.cpp"
    )
add_executable(testProj ${TESTPROJ_SRC})
target_link_libraries (testProj xg)
