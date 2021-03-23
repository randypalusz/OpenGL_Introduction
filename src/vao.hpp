#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "vbo.hpp"
#include "vbLayout.hpp"

class VertexArrayObject {
 public:
  VertexArrayObject();
  ~VertexArrayObject();
  // delete copy constructor
  VertexArrayObject(const VertexArrayObject&) = delete;
  void bind() const;
  void destroy();
  // using a const reference here to avoid the destructor being called
  // passing in just VertexBufferObject would make a copy (obviously including the
  // m_handle) and the destructor is called, deleting the vbo from the OpenGL context
  [[deprecated("Replaced by pushLayout")]] void setAttributes(
      const VertexBufferObject& vbo, unsigned int index, unsigned int numComponents,
      GLenum type, unsigned int stride, size_t offset);
  void pushLayout(const VertexBufferObject& vbo, const VertexBufferLayout& layout);

 private:
  unsigned int m_handle;
  unsigned int m_index = 0;
};

#endif