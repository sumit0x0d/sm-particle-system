cmake_minimum_required(VERSION 3.22.1)

project(sm-particle-system)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

# add_compile_options(-Wall -Wextra -g -Werror)

add_subdirectory(../sm-data-structures data-structures EXCLUDE_FROM_ALL)

add_library(sm-particle-system SHARED sm-particle-system.c glad.c)

target_include_directories(sm-particle-system PUBLIC sm-data-structures/include include)
target_link_libraries(sm-particle-system PUBLIC sm-dynamic-array)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
     enable_testing()
     add_executable(sm-particle-system-test sm-particle-system-test.c)
     target_link_libraries(sm-particle-system-test PUBLIC sm-particle-system glfw GL)
endif()
