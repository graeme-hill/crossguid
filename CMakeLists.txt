cmake_minimum_required(VERSION 3.5.1)
project(CrossGuid VERSION 0.2.3)

# ---- add dependencies via CPM ----

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME PackageProject.cmake
  GITHUB_REPOSITORY TheLartians/PackageProject.cmake
  VERSION 1.4
)

# ---- create target ----

option(CROSSGUID_TESTS "Build test runner" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_DEBUG_POSTFIX "-dgb")

# Set the build type if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

add_library(CrossGuid ${CMAKE_CURRENT_SOURCE_DIR}/src/guid.cpp)
set_property(TARGET CrossGuid PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(CrossGuid PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${PROJECT_NAME}-${PROJECT_VERSION}>)

if(WIN32)
    target_compile_definitions(CrossGuid PRIVATE GUID_WINDOWS)
elseif(APPLE)
    find_library(CFLIB CoreFoundation)
    target_link_libraries(CrossGuid ${CFLIB})
    target_compile_definitions(CrossGuid PRIVATE GUID_CFUUID)
elseif(ANDROID)
    # GUID_ANDROID is used in the headers, so make PUBLIC
    target_compile_definitions(CrossGuid PUBLIC GUID_ANDROID)
else()
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
    find_package(Libuuid REQUIRED)
    if (NOT LIBUUID_FOUND)
        message(FATAL_ERROR
            "You might need to run 'sudo apt-get install uuid-dev' or similar")
    endif()
    target_include_directories(CrossGuid PRIVATE ${LIBUUID_INCLUDE_DIR})
    target_link_libraries(CrossGuid ${LIBUUID_LIBRARY})
    target_compile_definitions(CrossGuid PRIVATE GUID_LIBUUID)
endif()

set_target_properties(CrossGuid
					  PROPERTIES
					  VERSION ${PROJECT_VERSION}
					  SOVERSION ${PROJECT_VERSION_MAJOR}
					  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  DEPENDENCIES ""
)
