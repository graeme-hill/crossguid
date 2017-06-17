# Author: Alexander Böhn (with Félix C. Morency)
# © 2011.10 -- GPL, Motherfuckers
cmake_minimum_required(VERSION 3.0)
project(libguid LANGUAGES C CXX)

# TODO: compile examples and tests (why not??)
# "testmain.cpp"
# "test.cpp"

set(libguid_VERSION_MAJOR "0")
set(libguid_VERSION_MINOR "3")
set(libguid_VERSION_PATCH "5")

add_definitions(
    -Wall -Werror
    -std=c++11 -stdlib=libc++ -O3
    -mtune=native -fstrict-aliasing)

if(APPLE)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    add_definitions(-DGUID_CFUUID)
endif(APPLE)

set(COMMON_LINK_FLAGS
    -m64)

set(srcs
    "guid.cpp")

foreach(FLAG IN LISTS ${COMMON_LINK_FLAGS})

    set_source_files_properties(
        ${srcs}
        PROPERTIES LINK_FLAGS ${LINK_FLAGS} ${FLAG})

endforeach()

set(hdrs
    "guid.h")

# add_library(libguid STATIC ${srcs} ${hdrs})
add_library(guid SHARED ${srcs} ${hdrs})

# set_target_properties(libguid
#     PROPERTIES ARCHIVE_OUTPUT_NAME "guid")
set_target_properties(guid
    PROPERTIES LIBRARY_OUTPUT_NAME "guid")
# target_link_libraries(libguid)
target_link_libraries(guid
    ${COREFOUNDATION_LIBRARY})

# install(TARGETS libguid DESTINATION lib)
install(TARGETS guid DESTINATION lib)
install(DIRECTORY ./ DESTINATION include/libguid
    FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ./ DESTINATION include/libguid
    FILES_MATCHING PATTERN "*.hh")
