#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <utility>

struct TextureParams {
  int width, height, numChannels;
  unsigned char* data;
};

class Texture {
 public:
  explicit Texture(const std::string& filePath);
  ~Texture();
  // delete copy constructor
  Texture(const Texture&) = delete;
  // move constructor - used for temporary creation of object to store in map
  // e.g. m_map.emplace("thing", Texture{"path"});
  Texture(Texture&& fromTex) {
    params = std::move(fromTex.params);
    m_handle = std::exchange(fromTex.m_handle, 0);
  }
  void bind() const;
  static void unbind();
  void setAttributes();

 private:
  void init();
  unsigned int loadTextureFromFile(const std::string& filePath);
  TextureParams params{};
  unsigned int m_handle = 0;
};
#endif