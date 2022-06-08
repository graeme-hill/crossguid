set(SOURCES
        guid.cpp)

add_library(CrossGuid ${SOURCES})

target_compile_features(CrossGuid PUBLIC cxx_std_17)
set_property(TARGET CrossGuid PROPERTY POSITION_INDEPENDENT_CODE ON)

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
    find_package(Libuuid REQUIRED)
    if (NOT LIBUUID_FOUND)
        message(FATAL_ERROR
                "You might need to run 'sudo apt-get install uuid-dev' or similar")
    endif()
    target_include_directories(CrossGuid PRIVATE ${LIBUUID_INCLUDE_DIR})
    target_link_libraries(CrossGuid ${LIBUUID_LIBRARY})
    target_compile_definitions(CrossGuid PRIVATE GUID_LIBUUID)
endif()

target_include_directories(CrossGuid
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

install(TARGETS CrossGuid
        EXPORT CrossGuid
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/include/crossguid/
        DESTINATION include/crossguid
        FILES_MATCHING PATTERN "*.hpp"
)