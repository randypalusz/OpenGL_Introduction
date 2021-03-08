#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum class VertexBufferType { VertexBuffer, IndexBuffer };

// TODO: add m_type for static/dynamic data type (last param in glBufferData)
class VertexBufferObject {
 public:
  VertexBufferObject(VertexBufferType type);
  ~VertexBufferObject();
  void bind() const;
  void destroy();
  void setAttributes(size_t offsetInBytes, size_t numBytes, void* data);

 private:
  unsigned int m_handle;
  // valid types are index/vertex for now
  GLenum m_type;
};

#endif