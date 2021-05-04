cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(containers VERSION 0.0.3 LANGUAGES C)

# Generate compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(containers INTERFACE)
add_library(CONTAINERS::containers ALIAS containers)
target_compile_features(containers INTERFACE c_std_11)
target_include_directories(containers INTERFACE
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

install(TARGETS containers EXPORT containers-targets)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT containers-targets
  FILE containers-targets.cmake
  NAMESPACE CONTAINERS::
  DESTINATION lib/cmake/containers
)
include(CMakePackageConfigHelpers)
configure_package_config_file(containers-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/containers-config.cmake
  INSTALL_DESTINATION lib/cmake/containers
  NO_SET_AND_CHECK_MACRO
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/containers-config.cmake
  DESTINATION lib/cmake/containers
)

enable_testing()
add_subdirectory(test)
