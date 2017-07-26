cmake_minimum_required(VERSION 3.5.1)
project(CrossGuid VERSION 0.2 LANGUAGES CXX)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

# Set GUID implementation per OS
if(WIN32)
elseif(APPLE)
    find_library(CFLIB CoreFoundation)
else()
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
    find_package(Libuuid REQUIRED)
    if (NOT LIBUUID_FOUND)
        message(FATAL_ERROR
            "You might need to run 'sudo apt-get install uuid-dev' or similar")
    endif()
endif()
# Targets to develop
enable_testing()
add_subdirectory(xg)
