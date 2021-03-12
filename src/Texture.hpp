#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

struct TextureParams {
  int width, height, numChannels;
  unsigned char* data;
};

// TODO: 3D texture support - only doing 2D for now
class Texture {
 public:
  explicit Texture(const std::string& filePath);
  ~Texture();
  // delete copy constructor
  Texture(const Texture&) = delete;
  void bind() const;
  static void unbind();
  void setAttributes();

 private:
  unsigned int init();
  unsigned int loadTextureFromFile(const std::string& filePath);
  TextureParams params{};
  unsigned int m_handle = 0;
};
#endif