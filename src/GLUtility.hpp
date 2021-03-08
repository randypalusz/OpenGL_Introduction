#ifndef GL_MY_UTILITY_HPP
#define GL_MY_UTILITY_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar* message,
                                const void* userParam);

#endif