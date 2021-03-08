#include "vbo.hpp"

#include <iostream>

VertexBufferObject::VertexBufferObject(VertexBufferType type) {
  m_type = (type == VertexBufferType::VertexBuffer) ? GL_ARRAY_BUFFER
                                                    : GL_ELEMENT_ARRAY_BUFFER;
  glGenBuffers(1, &m_handle);
  // std::cout << "Generating vbo, handleID: " << m_handle << std::endl;
}

VertexBufferObject::~VertexBufferObject() { this->destroy(); }

void VertexBufferObject::bind() const {
  // std::cout << "Binding vbo, handleID: " << m_handle << std::endl;
  glBindBuffer(m_type, m_handle);
}

void VertexBufferObject::destroy() {
  // std::cout << "Destroying vbo, handleID: " << m_handle << std::endl;
  glDeleteVertexArrays(1, &m_handle);
  m_handle = 0;
}

void VertexBufferObject::setAttributes(size_t offsetBytes, size_t numBytes, void* data) {
  // std::cout << "Setting vbo attr, handleID: " << m_handle << std::endl;
  this->bind();
  glBufferData(m_type, numBytes - offsetBytes, data, GL_STATIC_DRAW);
}