cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(hello LANGUAGES C CXX)
set(HELLO_MAJOR_VERSION 0)
set(HELLO_MINOR_VERSION 1)
SET(HELLO_VERSION ${HELLO_MINOR_VERSION}.${HELLO_MAJOR_VERSION})

message(STATUS "is the C++ compiler loaded? ${CMAKE_CXX_COMPILER_LOADED}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 99)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(PkgConfig REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_library("glad" "${CMAKE_SOURCE_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${CMAKE_SOURCE_DIR}/include")
set(LINK_LIBS ${OPENGL_gl_LIBRARY} glfw dl)


set(SOURCES
  hello
  hello_triangle
  rectangle
  triangle_ex1
  triangle_ex2
  triangle_ex3
  shader1
  shader2
  shader3
  shader_class
  shader_ex1
  shader_ex2
  shader_ex3
  texture1
  texture2
  transformation1
  transformation2
  trans_ex1
  trans_ex2
  coord1
  coord2
  coord3
  coord_ex1
  coord_ex2
  coord_ex3
  camera1
  camera2
  camera3
  camera4
  )

foreach(SOURCE ${SOURCES})
    add_executable(${SOURCE} ${CMAKE_SOURCE_DIR}/src/${SOURCE}.cpp)
    target_link_libraries(${SOURCE} "glad" ${LINK_LIBS})
endforeach(SOURCE)

configure_file(${CMAKE_SOURCE_DIR}/src/vs1.glsi ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/fs1.glsi ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/vsoff.glsi ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/vsex3.glsl ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/fsex3.glsl ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/vstex1.glsl ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/fstex1.glsl ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/vstex2.glsl ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/fstex2.glsl ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/fstex3.glsl ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/trans1.vs ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/trans1.fs ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/coord1.vs ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/coord1.fs ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/coord2.vs ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/coord2.fs ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/resources/container.jpg
  ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/resources/awesomeface.png
  ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
