#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLUtility.hpp"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar* message,
                                const void* userParam) {
  fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
          (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

auto shaderTypeToString(GLenum shaderType) -> std::string {
  std::string strType;
  switch (shaderType) {
    case GL_VERTEX_SHADER:
      strType = "GL_VERTEX_SHADER";
      break;
    case GL_FRAGMENT_SHADER:
      strType = "GL_FRAGMENT_SHADER";
      break;
    default:
      strType = "UNKNOWN SHADER TYPE - IMPLEMENT IN 'shaderTypeToString'";
      break;
  }
  return strType;
}