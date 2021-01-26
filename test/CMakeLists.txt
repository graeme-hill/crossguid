cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(CrossGuidTest LANGUAGES CXX)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

if(TEST_INSTALLED_VERSION)
  find_package(CrossGuid REQUIRED)
else()
  CPMAddPackage(NAME CrossGuid SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

CPMAddPackage(
  NAME Format.cmake
  GITHUB_REPOSITORY TheLartians/Format.cmake
  VERSION 1.6
  OPTIONS # enable cmake formatting
          "FORMAT_CHECK_CMAKE ON"
)

# ---- Create binary ----

add_executable(CrossGuidTest Test.cpp TestMain.cpp)
target_link_libraries(CrossGuidTest CrossGuid::CrossGuid)

set_target_properties(CrossGuidTest PROPERTIES CXX_STANDARD 17)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(CrossGuid PUBLIC -Wall -pedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(CrossGuid PUBLIC /W4 /WX)
    target_compile_definitions(CrossGuidTest PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# ---- Add CrossGuidTest ----

enable_testing()
ADD_TEST(CrossGuidTest CrossGuidTest)

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(CrossGuid PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(CrossGuid PUBLIC -fprofile-arcs -ftest-coverage)
endif()
