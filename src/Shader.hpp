#ifndef MY_SHADER_HPP
#define MY_SHADER_HPP

#include <string>

class ShaderUtils {
 public:
  static unsigned int CreateShader(const std::string& vertexShaderPath,
                                   const std::string& fragmentShaderPath);

 private:
  static unsigned int CompileShader(unsigned int type, const std::string& src);
  static std::string LoadShaderFromPath(const std::string& path);
  ShaderUtils() {}
};

#endif