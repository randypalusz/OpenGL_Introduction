#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "vbLayout.hpp"
#include "GLUtility.hpp"

template <typename T>
void VertexBufferLayout::push(int numElements) {
  size_t width = sizeof(T) * numElements;
  size_t size = sizeof(T);
  if (m_elements.size() == 0) {
    m_elements.emplace_back(LayoutElement{numElements, 0, size, width, typeToGL<T>()});
    m_stride += width;
  } else {
    LayoutElement& lastElement = m_elements.back();
    m_elements.emplace_back(
        LayoutElement{numElements, lastElement.width, size, width, typeToGL<T>()});
    m_stride += width;
  }
}

const std::vector<LayoutElement>& VertexBufferLayout::getElements() const {
  return m_elements;
}

const size_t VertexBufferLayout::getStride() const { return m_stride; }
