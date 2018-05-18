cmake_minimum_required(VERSION 3.5.1)
project(CrossGuid)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

option(CROSSGUID_TESTS "Build test runner" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_DEBUG_POSTFIX "-dgb")

# Set the build type if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

include(GNUInstallDirs)
set(CROSSGUID_INC_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
set(CROSSGUID_RUNTIME_INSTALL_DIR "${CMAKE_INSTALL_BINDIR}")
set(CROSSGUID_LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
set(CROSSGUID_ARCHIVE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
set(CROSSGUID_FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")

set(CROSSGUID_CMAKE_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/crossguid/cmake")
set(CROSSGUID_ADDITIONAL_FILES_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/crossguid")

add_library(crossguid ${CMAKE_CURRENT_SOURCE_DIR}/src/guid.cpp)
target_include_directories(crossguid PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

if(WIN32)
    target_compile_definitions(crossguid PRIVATE GUID_WINDOWS)
elseif(APPLE)
    find_library(CFLIB CoreFoundation)
    target_link_libraries(crossguid ${CFLIB})
    target_compile_definitions(crossguid PRIVATE GUID_CFUUID)
elseif(ANDROID)
    # GUID_ANDROID is used in the headers, so make PUBLIC
    target_compile_definitions(crossguid PUBLIC GUID_ANDROID)
else()
    find_package(Libuuid REQUIRED)
    if (NOT LIBUUID_FOUND)
        message(FATAL_ERROR
            "You might need to run 'sudo apt-get install uuid-dev' or similar")
    endif()
    target_include_directories(crossguid PRIVATE ${LIBUUID_INCLUDE_DIR})
    target_link_libraries(crossguid ${LIBUUID_LIBRARY})
    target_compile_definitions(crossguid PRIVATE GUID_LIBUUID)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(WARNINGS "-Werror" "-Wall")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(WARNINGS "-Werror" "-Wall")
elseif(MSVC)
    set(WARNINGS "/WX" "/W4")
endif()
target_compile_options(crossguid PRIVATE ${WARNINGS})

# Properties
# for a version number add
# VERSION ${CROSSGUID_VERSION_STRING} SOVERSION ${CROSSGUID_VERSION_STRING}
set_target_properties(crossguid
					  PROPERTIES
					  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Install target
install(TARGETS crossguid EXPORT crossguidTargets
    RUNTIME       DESTINATION ${CROSSGUID_RUNTIME_INSTALL_DIR}
    LIBRARY       DESTINATION ${CROSSGUID_LIBRARY_INSTALL_DIR}
    ARCHIVE       DESTINATION ${CROSSGUID_ARCHIVE_INSTALL_DIR}
    FRAMEWORK     DESTINATION ${CROSSGUID_FRAMEWORK_INSTALL_DIR})

# Install headers
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION ${CROSSGUID_INC_INSTALL_DIR}
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# Make cmake config files for all targets
install(EXPORT  crossguidTargets
    DESTINATION ${CROSSGUID_CMAKE_CONFIG_INSTALL_DIR}
    FILE crossguid-config.cmake)

# Install readme and license
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    DESTINATION ${CROSSGUID_ADDITIONAL_FILES_INSTALL_DIR}
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

if (CROSSGUID_TESTS)
    add_executable(crossguid-test test/TestMain.cpp test/Test.cpp)
    target_link_libraries(crossguid-test crossguid)
endif()
