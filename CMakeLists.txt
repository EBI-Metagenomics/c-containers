cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(container VERSION 0.0.1 LANGUAGES C)

# Generate compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(container INTERFACE)
add_library(CONTAINER::container ALIAS container)
target_compile_features(container INTERFACE c_std_11)
target_include_directories(container INTERFACE
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
set_target_properties(container PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS container EXPORT container-targets)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT container-targets
  FILE container-targets.cmake
  NAMESPACE CONTAINER::
  DESTINATION lib/cmake/container
)
include(CMakePackageConfigHelpers)
configure_package_config_file(container-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/container-config.cmake
  INSTALL_DESTINATION lib/cmake/container
  NO_SET_AND_CHECK_MACRO
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/container-config.cmake
  DESTINATION lib/cmake/container
)

enable_testing()
add_subdirectory(test)
