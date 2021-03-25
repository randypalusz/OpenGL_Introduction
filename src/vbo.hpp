#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

enum class VertexBufferType { VertexBuffer, IndexBuffer };

// TODO: add m_type for static/dynamic data type (last param in glBufferData)
class VertexBufferObject {
 public:
  VertexBufferObject(VertexBufferType type);
  ~VertexBufferObject();
  // delete copy constructor
  VertexBufferObject(const VertexBufferObject&) = delete;
  void bind() const;
  void destroy();
  [[deprecated("Replaced by setData")]] void setAttributes(size_t offsetInBytes,
                                                           size_t numBytes, void* data);

  // templated version of setAttributes assuming offset is 0,
  // and want to capture entire data array
  template <typename T>
  void setData(const std::vector<T>& data);

 private:
  unsigned int m_handle;
  // valid types are index/vertex for now
  GLenum m_type;
};

#endif