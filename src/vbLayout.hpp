#ifndef VB_LAYOUT_HPP
#define VB_LAYOUT_HPP

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct LayoutElement {
  size_t numElements;
  size_t offset;  // where first element starts ( in bytes )
  size_t size;    // width of a single element ( in bytes )
  size_t width;   // size * numElements
  GLenum type;
};

// go in order from first to last element when pushing
// TODO: maybe make instances of this part of the VertexBufferElement class?
class VertexBufferLayout {
 public:
  VertexBufferLayout() = default;
  ~VertexBufferLayout() = default;
  template <typename T>
  void push(size_t numElements);
  const std::vector<LayoutElement>& getElements() const;
  const size_t getStride() const;

 private:
  std::vector<LayoutElement> m_elements;
  size_t m_stride = 0;  // distance between consecutive elements ( in bytes )
};

#endif