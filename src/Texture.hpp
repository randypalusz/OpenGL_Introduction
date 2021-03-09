#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

class Texture {
 public:
  explicit Texture(const std::string& filePath);
  ~Texture();
  // delete copy constructor
  Texture(const Texture&) = delete;
  void bind() const;
  void setAttributes();

 private:
  unsigned int loadTextureFromFile(const std::string& filePath);
  unsigned int m_handle = 0;
};