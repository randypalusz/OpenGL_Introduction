#ifndef GL_MY_UTILITY_HPP
#define GL_MY_UTILITY_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Camera.hpp"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar* message,
                                const void* userParam);

void onWindowResize(GLFWwindow* window, int width, int height);

auto shaderTypeToString(GLenum shaderType) -> std::string;

#endif