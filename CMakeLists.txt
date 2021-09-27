cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(
  cco
  VERSION 0.0.6
  LANGUAGES C)

# Generate compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  # /wd5105:
  # https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/c5105?view=msvc-160
  set(WARNING_FLAGS /W3 /wd5105)
else()
  set(WARNING_FLAGS
      -Wall
      -Wextra
      -Wstrict-prototypes
      -Wshadow
      -Wconversion
      -Wmissing-prototypes
      -Wno-unused-parameter
      -Wsign-conversion
      -Wno-unused-function)

  if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
    list(APPEND WARNING_FLAGS -Wno-gnu-designator -Wno-empty-translation-unit
         -Wno-gnu-statement-expression -Wno-nullability-extension)
  endif()
endif()

option(CI_BUILD "CI, extra flags will be set" OFF)
if(CI_BUILD)
  message(STATUS "CI build enabled")
  if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/WX)
  else()
    add_compile_options(-Werror)
  endif()
endif()

add_library(cco INTERFACE)
add_library(CCO::cco ALIAS cco)
target_compile_features(cco INTERFACE c_std_11)
target_compile_options(cco INTERFACE ${WARNING_FLAGS})
target_include_directories(
  cco INTERFACE $<INSTALL_INTERFACE:include>
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

install(TARGETS cco EXPORT cco-targets)
install(DIRECTORY src/cco/ DESTINATION include/cco)
install(
  EXPORT cco-targets
  FILE cco-targets.cmake
  NAMESPACE CCO::
  DESTINATION lib/cmake/cco)
include(CMakePackageConfigHelpers)
configure_package_config_file(
  cco-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cco-config.cmake
  INSTALL_DESTINATION lib/cmake/cco
  NO_SET_AND_CHECK_MACRO)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cco-config.cmake
        DESTINATION lib/cmake/cco)

enable_testing()
add_subdirectory(test)
