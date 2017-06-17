cmake_minimum_required(VERSION 3.5.1)
project(CrossGuid)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

option(XG_TESTS "Build test runner" ON)

include_directories(.)
set(CMAKE_CXX_STANDARD 11)

add_library(xg Guid.cpp)

if (XG_TESTS)
	add_executable(xgtest test/TestMain.cpp test/Test.cpp)
	target_link_libraries(xgtest xg)
endif()

if(WIN32)

elseif(APPLE)

else()
	find_package(Libuuid REQUIRED)
	if (NOT LIBUUID_FOUND)
		message(FATAL_ERROR "You might need to run something like: sudo apt-get install uuid-dev")
	endif()
	include_directories(${LIBUUID_INCLUDE_DIR})
	target_link_libraries(xg ${LIBUUID_LIBRARY})
	add_definitions(-DGUID_LIBUUID)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()
