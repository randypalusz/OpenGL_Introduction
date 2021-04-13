cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

project(OpenGL_Test VERSION 0.01)

set(CMAKE_PROJECT_VERSION 0.1a)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(RES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res)

set(SOURCES
  ${SRC_DIR}/main.cc
  ${SRC_DIR}/Application.cpp
  ${SRC_DIR}/GLUtility.cpp
  ${SRC_DIR}/Shader.cpp
  ${SRC_DIR}/vao.cpp
  ${SRC_DIR}/vbo.cpp
  ${SRC_DIR}/Texture.cpp
  ${SRC_DIR}/vbLayout.cpp
  ${SRC_DIR}/OBBIntersection.cpp
  ${SRC_DIR}/ReactUtility.cpp
)

set(HEADERS 
  ${SRC_DIR}/GLUtility.hpp
  ${SRC_DIR}/Application.hpp
  ${SRC_DIR}/Shader.hpp
  ${SRC_DIR}/vao.hpp
  ${SRC_DIR}/vbo.hpp
  ${SRC_DIR}/Texture.hpp
  ${SRC_DIR}/shapes/Cube.hpp
  ${SRC_DIR}/input/Command.hpp
  ${SRC_DIR}/input/InputHandler.hpp
  ${SRC_DIR}/Camera.hpp
  ${SRC_DIR}/Timer.hpp
  ${SRC_DIR}/vbLayout.hpp
  ${SRC_DIR}/OBBIntersection.hpp
  ${SRC_DIR}/ReactUtility.hpp
)

# adding executable, changing executable suffix for windows
set(CMAKE_EXECUTABLE_SUFFIX ".cc")
set(${PROJECT_NAME} ${PROJECT_NAME}.cc)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_executable(Example ${SRC_DIR}/basic.cc ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")

## GLFW
# multi-platform friendly - compiles/links library depending on build system
set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
#
target_link_libraries(Example "glfw" "${GLFW_LIBRARIES}")
target_include_directories(Example PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(Example PRIVATE "GLFW_INCLUDE_NONE")

## glad
# https://github.com/aaronmjacobs/InitGL
# requires glad installation here: https://glad.dav1d.de/
#   choose c/c++, choose 3.3 gl API, profile = Core
# drop in lib/
set(GLAD_DIR ${LIB_DIR}/glad)
add_library("glad" ${GLAD_DIR}/src/glad.c)
target_include_directories("glad" PRIVATE ${GLAD_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLAD_DIR}/include)
target_link_libraries(${PROJECT_NAME} "glad" ${CMAKE_DL_LIBS})

target_include_directories(Example PRIVATE ${GLAD_DIR}/include)
target_link_libraries(Example "glad" ${CMAKE_DL_LIBS})

## glm
set(GLM_DIR ${LIB_DIR}/glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_DIR})

## stb_image (image library)
set(STB_DIR ${LIB_DIR}/stb)
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_DIR})

set(REACT_DIR "${LIB_DIR}/react")
add_subdirectory(${REACT_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${REACT_DIR}/include") 
target_link_libraries(${PROJECT_NAME} "reactphysics3d")

## copying shaders/textures into build directory
file(COPY ${RES_DIR} DESTINATION .)