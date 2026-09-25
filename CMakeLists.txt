cmake_minimum_required(VERSION 3.22.1)

project(sm-particle-system)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

# add_compile_options(-Wall -Wextra -g -Werror)
find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client)
add_subdirectory(../sm-data-structures sm-data-structures EXCLUDE_FROM_ALL)

add_library(sm-particle-system SHARED source/sm-particle-system.c source/glad.c)
target_include_directories(sm-particle-system PUBLIC sm-data-structures/include include ${WAYLAND_INCLUDE_DIRS})
target_link_libraries(sm-particle-system PUBLIC sm-dynamic-array)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
     enable_testing()
     add_executable(sm-particle-system-test test/sm-particle-system.c)
     target_link_libraries(sm-particle-system-test PUBLIC sm-particle-system glfw GL ${WAYLAND_LIBRARIES})
endif()
