cmake_minimum_required(VERSION 3.5.1)
project(CrossGuid)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

option(XG_TESTS "Build test runner" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_DEBUG_POSTFIX "-dgb")

# Define Install Directories
# Install stuff (default location is not some where on the system! for safty reasons)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE STRING "Install prefix (e.g. /usr/local/)" FORCE)
endif()
if(UNIX)
    include(GNUInstallDirs)
    set(XG_INC_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}") 
    set(XG_RUNTIME_INSTALL_DIR "${CMAKE_INSTALL_BINDIR}") 
    set(XG_LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set(XG_ARCHIVE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set(XG_FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")

    set(XG_CMAKE_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/crossguid/cmake")
    set(XG_ADDITIONAL_FILES_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/crossguid")
elseif(WIN32)
    set(XG_INC_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}") 
    set(XG_RUNTIME_INSTALL_DIR   "bin") 
    set(XG_LIBRARY_INSTALL_DIR   "bin")
    set(XG_ARCHIVE_INSTALL_DIR   "lib")
    set(XG_FRAMEWORK_INSTALL_DIR "bin")

    set(XG_CMAKE_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/cmake")
    set(XG_ADDITIONAL_FILES_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")
else()
	message(FATAL_ERROR "Could not set install folders for this platform!")
endif()

add_library(xg STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Guid.cpp)
target_include_directories(xg PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
									 $<INSTALL_INTERFACE:include>)

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

# Properties 
# for a version number add 
# VERSION ${XG_VERSION_STRING} SOVERSION ${XG_VERSION_STRING}
set_target_properties(xg 
					  PROPERTIES
					  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
					  OUTPUT_NAME "crossguid")

# Install target
install(TARGETS xg EXPORT XGTargets
		RUNTIME       DESTINATION ${XG_RUNTIME_INSTALL_DIR}
		LIBRARY       DESTINATION ${XG_LIBRARY_INSTALL_DIR}
		ARCHIVE       DESTINATION ${XG_ARCHIVE_INSTALL_DIR}
		FRAMEWORK     DESTINATION ${XG_FRAMEWORK_INSTALL_DIR})

# Install headers
message(STATUS ${XG_INC_INSTALL_DIR})
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" 
		DESTINATION ${XG_INC_INSTALL_DIR}
		FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        DIRECTORY_PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# Make cmake config files for all targets
install(EXPORT  XGTargets
		DESTINATION ${XG_CMAKE_CONFIG_INSTALL_DIR}
		NAMESPACE crossguid::
		FILE crossguid-config.cmake)

# Install readme and license
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
        DESTINATION ${XG_ADDITIONAL_FILES_INSTALL_DIR}
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

# Write/install version file [not used yet]
# if we have version inclue this:
# include(CMakePackageConfigHelpers)
# write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/cmake/crossguid-config-version.cmake"
#                                  VERSION ${XG_VERSION_STRING}
#                                  COMPATIBILITY AnyNewerVersion)
# install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cmake/crossguid-config-version.cmake"
#         DESTINATION ${XG_CMAKE_CONFIG_INSTALL_DIR})

if (XG_TESTS)
	add_executable(xgtest test/TestMain.cpp test/Test.cpp)
	target_link_libraries(xgtest xg)
endif()

