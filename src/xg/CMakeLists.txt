project(libxg VERSION 0.2 LANGUAGES CXX)

add_library(xg
    src/xg.cpp)
    
# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(xg PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

# Set GUID implementation per OS and link libraries
if(WIN32)
	add_definitions(-DGUID_WINDOWS)
elseif(APPLE)
	target_link_libraries(xg ${CFLIB})
	add_definitions(-DGUID_CFUUID)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
else()
    include_directories(${LIBUUID_INCLUDE_DIR})
    target_link_libraries(xg ${LIBUUID_LIBRARY})
	add_definitions(-DGUID_LIBUUID)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

# 'make install' to the correct locations (provided by GNUInstallDirs).
install(TARGETS xg EXPORT xgConfig
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# This makes the project importable from the install directory
# Put config file in per-project dir (name MUST match), can also
# just go into 'cmake'.
install(EXPORT xgConfig DESTINATION share/xg/cmake)

# This makes the project importable from the build directory
export(TARGETS xg FILE xgConfig.cmake)

# Set flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_STANDARD 11)

# Unit tests for xg
add_executable(testxg
    test/testxg.cpp)

target_link_libraries(testxg
    xg)
add_test(testxg testxg)

