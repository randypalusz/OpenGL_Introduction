#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <assert.h>
#include <iostream>

#include "Texture.hpp"

Texture::Texture(const std::string& filePath) {
  unsigned int success = loadTextureFromFile(filePath);
  assert(success != 0);
  this->init();
}

Texture::~Texture() {  // glDeleteTextures(1, &m_handle);
}

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, m_handle); }

void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void Texture::init() {
  // init assumes the texture could be generated
  glGenTextures(1, &m_handle);
  this->bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, params.width, params.height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, params.data);

  glGenerateMipmap(GL_TEXTURE_2D);
}

unsigned int Texture::loadTextureFromFile(const std::string& filePath) {
  params.data =
      stbi_load(filePath.c_str(), &params.width, &params.height, &params.numChannels, 0);
#ifndef NDEBUG
  std::cout << "Loaded Texture data from file: " << filePath << std::endl;
#endif
  return (params.data == nullptr ? 0 : 1);
}