include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()

set(SOURCES
  CrossGuidTests.cpp
  main.cpp
)

add_executable(CrossGuid_Tests ${SOURCES})
add_dependencies(CrossGuid_Tests CrossGuid)
target_compile_features(CrossGuid_Tests PUBLIC cxx_std_17)
target_link_libraries(CrossGuid_Tests
  PUBLIC
    CrossGuid
    gtest_main
)

gtest_discover_tests(CrossGuid_Tests
  EXTRA_ARGS "--gtest_output=xml:${CMAKE_CURRENT_BINARY_DIR}/../results/")