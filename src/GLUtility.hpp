#ifndef GL_UTILITY_HPP
#define GL_UTILITY_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar* message,
                                const void* userParam) {
  fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
          (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

#endif