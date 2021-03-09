#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture.hpp"

#include <iostream>

Texture::Texture(const std::string& filePath) {
  m_handle = loadTextureFromFile(filePath);
}

Texture::~Texture() { glDeleteTextures(1, &m_handle); }

unsigned int Texture::loadTextureFromFile(const std::string& filePath) {
  // TODO: actually load something, returning junk so this thing compiles
  return 1;
}