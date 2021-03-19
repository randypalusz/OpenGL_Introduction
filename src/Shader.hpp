#ifndef MY_SHADER_HPP
#define MY_SHADER_HPP
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <utility>

struct UniformParams {
  GLenum type;
  int location;
};

class Shader {
 public:
  explicit Shader(const std::string& vertexShaderPath,
                  const std::string& fragmentShaderPath);
  // delete copy constructor
  Shader(const Shader&) = delete;
  // move constructor - used for temporary creation of object to store in map
  Shader(Shader&& fromShader) {
    m_handle = std::exchange(fromShader.m_handle, 0);
    m_uniformMap = std::move(fromShader.m_uniformMap);
  }
  void use() const;
  /// \brief Templated setUniform function for glm types
  template <typename T>
  void setUniform(const std::string& uniformName, const T& val);
  /// \brief setUniform override for a single float
  void setUniform(const std::string& uniformName, const float& val);

 private:
  unsigned int CompileShader(unsigned int type, const std::string& src);
  /// \brief returns type and location of uniform, handles map insert/get
  UniformParams& getUniformParams(const std::string& uniformName);
  std::string LoadShaderFromPath(const std::string& path);
  unsigned int m_handle;
  // index/type
  std::unordered_map<std::string, UniformParams> m_uniformMap;
};

#endif