#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <assert.h>
#include <fstream>

#include "Shader.hpp"
#include "GLUtility.hpp"

auto ShaderUtils::CreateShader(const std::string& vsPath, const std ::string& fsPath)
    -> unsigned int {
  std::string vs = LoadShaderFromPath(vsPath);
  std::string fs = LoadShaderFromPath(fsPath);
  unsigned int program = glCreateProgram();
  unsigned int vsCompiled = CompileShader(GL_VERTEX_SHADER, vs);
  unsigned int fsCompiled = CompileShader(GL_FRAGMENT_SHADER, fs);

  assert(vsCompiled != 0);
  assert(fsCompiled != 0);

  glAttachShader(program, vsCompiled);
  glAttachShader(program, fsCompiled);

  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vsCompiled);
  glDeleteShader(fsCompiled);
  return program;
}

auto ShaderUtils::CompileShader(unsigned int shaderType, const std::string& shaderCode)
    -> unsigned int {
  unsigned int id = glCreateShader(shaderType);
  const char* src = shaderCode.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to Compile " << shaderTypeToString(shaderType)
              << ", message: " << message << std::endl;
    std::cout << "Shader Source: \n" << shaderCode << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

auto ShaderUtils::LoadShaderFromPath(const std::string& path) -> std::string {
  std::ifstream ifs(path);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      std::istreambuf_iterator<char>());
  return content;
}