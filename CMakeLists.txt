cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(
  cco
  VERSION 0.0.6
  LANGUAGES C)

# Generate compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(cco INTERFACE)
add_library(CCO::cco ALIAS cco)
target_compile_features(cco INTERFACE c_std_11)
target_include_directories(
  cco INTERFACE $<INSTALL_INTERFACE:include>
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

install(TARGETS cco EXPORT cco-targets)
install(DIRECTORY include/ DESTINATION include)
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
