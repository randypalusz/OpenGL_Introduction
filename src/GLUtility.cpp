#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <chrono>

#include "GLUtility.hpp"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar* message,
                                const void* userParam) {
  std::string sevString = "";
  std::string errorTypeString = "";
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      sevString = "HIGH";
      break;
    case GL_DEBUG_SEVERITY_LOW:
      sevString = "LOW";
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      sevString = "MEDIUM";
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      sevString = "NOTIFICATION";
      break;
    default:
      sevString = "UNKNOWN SEVERITY";
      break;
  }

  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      errorTypeString = "GL_DEBUG_TYPE_ERROR";
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      errorTypeString = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
      break;
    case GL_DEBUG_TYPE_MARKER:
      errorTypeString = "GL_DEBUG_TYPE_MARKER";
      break;
    case GL_DEBUG_TYPE_OTHER:
      errorTypeString = "GL_DEBUG_TYPE_OTHER";
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      errorTypeString = "GL_DEBUG_TYPE_PERFORMANCE";
      break;
    case GL_DEBUG_TYPE_POP_GROUP:
      errorTypeString = "GL_DEBUG_TYPE_POP_GROUP";
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      errorTypeString = "GL_DEBUG_TYPE_PORTABILITY";
      break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
      errorTypeString = "GL_DEBUG_TYPE_PUSH_GROUP";
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      errorTypeString = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
      break;
    default:
      errorTypeString = "UNKNOWN ERROR TYPE";
      break;
  }
  fprintf(stderr, "GL CALLBACK: type = %s, severity = %s, message = %s\n",
          errorTypeString.c_str(), sevString.c_str(), message);
}

void onWindowResize(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
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

template <typename T>
GLenum typeToGL() {
  switch (typeid(T)) {
    case typeid(float):
      return GL_FLOAT;
    case typeid(double):
      return GL_DOUBLE;
    case typeid(char):
      return GL_BYTE;
    case typeid(unsigned char):
      return GL_UNSIGNED_BYTE;
    case typeid(int):
      return GL_INT;
    case typeid(unsigned int):
      return GL_UNSIGNED_INT;
    case typeid(short):
      return GL_SHORT;
    case typeid(unsigned short):
      return GL_UNSIGNED_SHORT;
    default:
      return GL_INVALID_ENUM;
  }
}