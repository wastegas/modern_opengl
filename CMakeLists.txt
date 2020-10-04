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

add_executable(hello ${CMAKE_SOURCE_DIR}/src/hello.cpp)
add_executable(hello_triangle ${CMAKE_SOURCE_DIR}/src/hello_triangle.cpp)
add_executable(rectangle ${CMAKE_SOURCE_DIR}/src/rectangle.cpp)
add_executable(triangle_ex1 ${CMAKE_SOURCE_DIR}/src/triangle_ex1.cpp)
add_executable(triangle_ex2 ${CMAKE_SOURCE_DIR}/src/triangle_ex2.cpp)
add_executable(triangle_ex3 ${CMAKE_SOURCE_DIR}/src/triangle_ex3.cpp)
add_executable(shader1 ${CMAKE_SOURCE_DIR}/src/shader1.cpp)

add_library("glad" "${CMAKE_SOURCE_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${CMAKE_SOURCE_DIR}/include")
set(LINK_LIBS ${OPENGL_gl_LIBRARY} glfw dl)
target_link_libraries(hello "glad" ${LINK_LIBS})
target_link_libraries(hello_triangle "glad" ${LINK_LIBS})
target_link_libraries(rectangle "glad" ${LINK_LIBS})
target_link_libraries(triangle_ex1 "glad" ${LINK_LIBS})
target_link_libraries(triangle_ex2 "glad" ${LINK_LIBS})
target_link_libraries(triangle_ex3 "glad" ${LINK_LIBS})
target_link_libraries(shader1 "glad" ${LINK_LIBS})
			  
