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

template <typename T>
void Shader::setUniform(const std::string& uniformName, const T& val) {
  UniformParams p = getUniformParams(uniformName);
  assert(p.location != -1);
  switch (p.type) {
    case GL_FLOAT:
      glUniform1fv(p.location, 1, glm::value_ptr(val));
      break;
    case GL_FLOAT_VEC2:
      glUniform2fv(p.location, 1, glm::value_ptr(val));
      break;
    case GL_FLOAT_VEC3:
      glUniform3fv(p.location, 1, glm::value_ptr(val));
      break;
    case GL_FLOAT_VEC4:
      glUniform4fv(p.location, 1, glm::value_ptr(val));
      break;
    case GL_FLOAT_MAT2:
      glUniformMatrix4fv(p.location, 1, GL_FALSE, glm::value_ptr(val));
      break;
    case GL_FLOAT_MAT3:
      glUniformMatrix4fv(p.location, 1, GL_FALSE, glm::value_ptr(val));
      break;
    case GL_FLOAT_MAT4:
      glUniformMatrix4fv(p.location, 1, GL_FALSE, glm::value_ptr(val));
      break;
    default:
#ifndef NDEBUG
      std::cout << "Uniform '" << uniformName
                << "' not set. May need definition in Shader::setUniform()" << std::endl;
#endif
      break;
  }
}

void Shader::setUniform(const std::string& uniformName, const float& val) {
  UniformParams p = getUniformParams(uniformName);
  assert(p.location != -1);
  glUniform1f(p.location, val);
}

// forward declaring the potential defiintions of this template here...
// template void Shader::setUniform<float>(const std::string&, const float&) const;
template void Shader::setUniform<glm::vec2>(const std::string&, const glm::vec2&);
template void Shader::setUniform<glm::vec3>(const std::string&, const glm::vec3&);
template void Shader::setUniform<glm::vec4>(const std::string&, const glm::vec4&);
template void Shader::setUniform<glm::mat4>(const std::string&, const glm::mat4&);

UniformParams& Shader::getUniformParams(const std::string& uniformName) {
  GLenum type;
  GLint size;
  GLsizei length;
  int location = -1;
  // if uniform is found in map
  if (m_uniformMap.find(uniformName) == m_uniformMap.end()) {
    location = glGetUniformLocation(m_handle, uniformName.c_str());
    glGetActiveUniform(m_handle, (GLuint)location, (GLsizei)0, &length, &size, &type,
                       nullptr);
    m_uniformMap.insert({uniformName, UniformParams{type, location}});
  }

  return m_uniformMap.at(uniformName);
}