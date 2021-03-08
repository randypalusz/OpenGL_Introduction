#include "vao.hpp"
#include "vbo.hpp"

#include <iostream>

VertexArrayObject::VertexArrayObject() {
  glGenVertexArrays(1, &m_handle);
  // std::cout << "Generating vao, handleID: " << m_handle << std::endl;
}

VertexArrayObject::~VertexArrayObject() { destroy(); }

void VertexArrayObject::bind() const {
  // std::cout << "Binding vao, handleID: " << m_handle << std::endl;
  glBindVertexArray(m_handle);
}

void VertexArrayObject::destroy() {
  // std::cout << "Destroying vao, handleID: " << m_handle << std::endl;
  glDeleteVertexArrays(1, &m_handle);
  m_handle = 0;
}

void VertexArrayObject::setAttributes(const VertexBufferObject& vbo, unsigned int index,
                                      unsigned int numComponents, GLenum type,
                                      unsigned int stride, size_t offset) {
  // std::cout << "Setting vao attr, handleID: " << m_handle << std::endl;
  this->bind();
  vbo.bind();

  // TODO: handle integer call (glVertexAttribIPointer) for Integer types
  glVertexAttribPointer(index, numComponents, type, GL_FALSE, stride,
                        (const void*)offset);

  glEnableVertexAttribArray(index);
}
