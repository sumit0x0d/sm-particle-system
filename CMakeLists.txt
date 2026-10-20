cmake_minimum_required(VERSION 3.22.1)

project(sm-particle-system)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_library(sm-particle-system SHARED
     source/sm-particle-system.c
     source/sm-particle-system-particle-array.c
     source/sm-particle-system-opengl.c
     third-party/glad/glad.c)
target_include_directories(sm-particle-system
     PUBLIC include ../sm-core/include
     PRIVATE third-party/glad)
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/shaders")
configure_file(source/shaders/vertex-shader.glsl
     "${CMAKE_CURRENT_BINARY_DIR}/shaders/vertex-shader.glsl" COPYONLY)
configure_file(source/shaders/fragment-shader.glsl
     "${CMAKE_CURRENT_BINARY_DIR}/shaders/fragment-shader.glsl" COPYONLY)
target_compile_definitions(sm-particle-system PRIVATE
     SM_PARTICLE_SYSTEM_SHADER_DIR="${CMAKE_CURRENT_BINARY_DIR}/shaders")
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
     enable_testing()
     add_executable(sm-particle-system-test test/sm-particle-system.c)
     target_link_libraries(sm-particle-system-test PUBLIC sm-particle-system glfw GL ${WAYLAND_LIBRARIES})
     add_executable(sm-particle-system-particle-array-test test/sm-particle-system-particle-array.c)
     target_include_directories(sm-particle-system-particle-array-test PRIVATE source)
     target_link_libraries(sm-particle-system-particle-array-test PRIVATE sm-particle-system)
     add_test(NAME sm-particle-system-particle-array-test COMMAND sm-particle-system-particle-array-test)
endif()
