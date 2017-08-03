cmake_minimum_required(VERSION 3.5.1)
project(CrossGuid)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

option(XG_TESTS "Build test runner" ON)

include_directories(.)
set(CMAKE_CXX_STANDARD 11)

add_library(xg Guid.cpp)

if (XG_TESTS)
	add_executable(xgtest test/TestMain.cpp test/Test.cpp)
	target_link_libraries(xgtest xg)
endif()


if(WIN32)
	add_definitions(-DGUID_WINDOWS)
elseif(APPLE)
	find_library(CFLIB CoreFoundation)
	target_link_libraries(xg ${CFLIB})
	add_definitions(-DGUID_CFUUID)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
elseif(ANDROID)
	target_compile_definitions(xg PRIVATE GUID_ANDROID)
else()
	find_package(Libuuid REQUIRED)
	if (NOT LIBUUID_FOUND)
		message(FATAL_ERROR
			"You might need to run 'sudo apt-get install uuid-dev' or similar")
	endif()
	include_directories(${LIBUUID_INCLUDE_DIR})
	target_link_libraries(xg ${LIBUUID_LIBRARY})
	add_definitions(-DGUID_LIBUUID)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()
