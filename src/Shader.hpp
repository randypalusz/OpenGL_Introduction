#ifndef MY_SHADER_HPP
#define MY_SHADER_HPP
#include <glm/glm.hpp>

#include <string>

class Shader {
 public:
  Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
  void use() const;
  /// \brief Templated setUniform function for glm types
  template <typename T>
  void setUniform(const std::string& uniformName, const T& val) const;
  /// \brief setUniform override for a single float
  void setUniform(const std::string& uniformName, const float& val) const;
  void setUniform4f(const std::string& uniformName, const glm::vec4& vals) const;
  void setUniformMatrix4fv(const std::string& uniformName, const glm::mat4& matrix) const;
  void setUniform1f(const std::string& uniformName, const float val) const;

 private:
  unsigned int CompileShader(unsigned int type, const std::string& src);
  std::string LoadShaderFromPath(const std::string& path);
  unsigned int m_handle;
};

#endif