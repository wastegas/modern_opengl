cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(modernopengl)
set(MODERNOPENGL_MAJOR_VERSION 0)
set(MODERNOPENGL_MINOR_VERSION 1)
SET(MODERNOPENGL_VERSION ${MODERNOPENGL_MINOR_VERSION}.
  ${MODERNOPENGL_MAJOR_VERSION})

message(STATUS "is the C++ compiler loaded? ${CMAKE_CXX_COMPILER_LOADED}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 99)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(PkgConfig REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_library("glad" "${CMAKE_SOURCE_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${CMAKE_SOURCE_DIR}/include")
set(LINK_LIBS ${OPENGL_gl_LIBRARY} glfw dl assimp)


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
  textex1
  textex2
  textex3
  textex4
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
  camera_class
  colors
  ambient
  diffuse
  specular
  specular_ex1
  material1
  material2
  material3
  material_ex1
  map_diffuse
  map_specular
  map_specular_ex2
  map_specular_ex3
  map_specular_ex4
  castDirection
  castPoint
  castSpot
  castSpotSoft
  castMultiple
  modelLoading
  girl
  note
  star
  depthTest
  depthBuffer
  stencil
  blendDiscard
  blendSort
  faceCulling
  )

foreach(SOURCE ${SOURCES})
    add_executable(${SOURCE} ${CMAKE_SOURCE_DIR}/src/${SOURCE}.cpp)
    target_link_libraries(${SOURCE} "glad" ${LINK_LIBS})
endforeach(SOURCE)

file(GLOB SHADERS
  "${CMAKE_SOURCE_DIR}/src/*.vs"
  "${CMAKE_SOURCE_DIR}/src/*.fs"
  )

foreach(SHADER ${SHADERS})
  configure_file(${SHADER} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach(SHADER)


file(GLOB TEXTURES
  "${CMAKE_SOURCE_DIR}/resources/*.jpg"
  "${CMAKE_SOURCE_DIR}/resources/*.png"
  )

file(GLOB MODELS
  "${CMAKE_SOURCE_DIR}/resources/models/backpack/*.*"
  "${CMAKE_SOURCE_DIR}/resources/models/girl/*.*"
  "${CMAKE_SOURCE_DIR}/resources/models/note/*.*"
    "${CMAKE_SOURCE_DIR}/resources/models/star/*.*"
  )

foreach(TEXTURE ${TEXTURES})
  configure_file(${TEXTURE} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach(TEXTURE)

foreach(MODEL ${MODELS})
  configure_file(${MODEL} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach(MODEL)
