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

/// \brief Adds an attribute pointer to the VAO based on the reference vbo
///
/// \param vbo The input VBO containing the data (positions, normals, tex coords, etc...)
/// \param index The desired index in the VAO to bind this data to
/// \param numComponents
/// Number of components per unique element --
/// Regardless of how many unique elements there are (may be x number of unique positions,
/// for example), this refers to the number of components in one of those positions
/// Ex. 15 unique positions in 2D space (x, y), this would be set to 2
/// \param type Refers to the GL datatype of each component
/// \param stride The distance between consecutive elements (in Bytes)
/// \param offset The position in the data array of the first occurrence of this attribute
/// (in Bytes)
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
