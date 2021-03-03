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

set(SOURCES
  ${SRC_DIR}/main.cc
)

set(HEADERS 
  ${SRC_DIR}/node.h
)

# adding executable, changing executable suffix for windows
set(CMAKE_EXECUTABLE_SUFFIX ".cc")
set(${PROJECT_NAME} ${PROJECT_NAME}.cc)
add_executable(${PROJECT_NAME} ${SOURCES})
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