#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <assert.h>
#include <fstream>

#include "Shader.hpp"
#include "GLUtility.hpp"

Shader::Shader(const std::string& vertexShaderPath,
               const std::string& fragmentShaderPath) {
  std::string vs = LoadShaderFromPath(vertexShaderPath);
  std::string fs = LoadShaderFromPath(fragmentShaderPath);
  m_handle = glCreateProgram();
  unsigned int vsCompiled = CompileShader(GL_VERTEX_SHADER, vs);
  unsigned int fsCompiled = CompileShader(GL_FRAGMENT_SHADER, fs);

  assert(vsCompiled != 0);
  assert(fsCompiled != 0);

  glAttachShader(m_handle, vsCompiled);
  glAttachShader(m_handle, fsCompiled);

  glLinkProgram(m_handle);
  glValidateProgram(m_handle);

  glDeleteShader(vsCompiled);
  glDeleteShader(fsCompiled);
}

void Shader::use() const { glUseProgram(m_handle); }

auto Shader::CompileShader(unsigned int shaderType, const std::string& shaderCode)
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
    free(message);
    glDeleteShader(id);
    return 0;
  }

  return id;
}

auto Shader::LoadShaderFromPath(const std::string& path) -> std::string {
  std::ifstream ifs(path);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      std::istreambuf_iterator<char>());
  return content;
}

void Shader::setUniform4f(const std::string& uniformName, const glm::vec4& vals) const {
  int location = glGetUniformLocation(m_handle, uniformName.c_str());
  assert(location != -1);
  glUniform4f(location, vals.x, vals.y, vals.z, vals.w);
}

void Shader::setUniformMatrix4fv(const std::string& uniformName,
                                 const glm::mat4& matrix) const {
  int location = glGetUniformLocation(m_handle, uniformName.c_str());
  assert(location != -1);
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform1f(const std::string& uniformName, float val) const {
  int location = glGetUniformLocation(m_handle, uniformName.c_str());
  assert(location != -1);
  glUniform1f(location, val);
}