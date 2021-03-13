#ifndef SQUARE_HPP
#define SQUARE_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vao.hpp"
#include "vbo.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

// Singleton that will hold common square attributes (index buffer, vbo)
class CommonSquareAttributes {
 public:
  static CommonSquareAttributes& get() {
    static CommonSquareAttributes instance;
    return instance;
  }
  CommonSquareAttributes(CommonSquareAttributes const&) = delete;
  void operator=(CommonSquareAttributes const&) = delete;
  float* positions;
  unsigned int* indices;
  VertexBufferObject ibo{VertexBufferType::IndexBuffer};
  VertexBufferObject vbo{VertexBufferType::VertexBuffer};

 private:
  CommonSquareAttributes() {
    positions = new float[16]{
        // positions  // tex coords
        -0.5f, -0.5f, 0.0f, 0.0f,  // vertex 1
        0.5f,  -0.5f, 1.0f, 0.0f,  // vertex 2
        0.5f,  0.5f,  1.0f, 1.0f,  // vertex 3
        -0.5f, 0.5f,  0.0f, 1.0f   // vertex 4
    };
    indices = new unsigned int[6]{
        0, 1, 2,  // triangle 1
        2, 3, 0   // triangle 2
    };
    // (starts at index 0, length = 6 unsigned ints, data is in the indices array)
    ibo.setAttributes(0, sizeof(unsigned int) * 6, indices);
    // (starts at index 0, length = 16 floats, data is in the positions array)
    vbo.setAttributes(0, sizeof(float) * 16, positions);
  };
};

class Square {
 public:
  Square(Shader* shader, Texture* texture) {
    m_texture = texture;
    m_shader = shader;
    // (bind vertex buffer, choose index 0, 2 elements for this attribute(x,y), float,
    //  distance from this attribute to the next one is the length of 4 floats in bytes,
    //  the first position starts at index 0 in the vertex array)
    m_vao.setAttributes(m_vbo, 0, 2, GL_FLOAT, sizeof(float) * 4, 0);

    // (bind vertex buffer, choose index 1, 2 elements for this attribute(x,y), float,
    //  distance from this attribute to the next one is the length of 4 floats in bytes,
    //  the first position starts at index 2 in the index array)
    m_vao.setAttributes(m_vbo, 1, 2, GL_FLOAT, sizeof(float) * 4, 2 * sizeof(float));

    // (bind index buffer, choose index 2, 1 element for this attribute (just index),
    // uint,
    //  distance from this attribute to the next one is the length of one uint in bytes,
    //  the first position starts at index 0 in the index array)
    m_vao.setAttributes(m_ibo, 2, 1, GL_UNSIGNED_INT, sizeof(unsigned int), 0);
  }
  void draw() {
    // enable correct shaders
    m_shader->use();

    // set color uniform for active shader
    m_shader->setUniform4f("u_Color", m_color);

    // set gradient uniform for active shader
    m_shader->setUniform1f("u_enableBlueGradient", m_enableBlueGradient);

    // set transform matrix
    m_shader->setUniformMatrix4fv("u_transformMatrix", m_transformMatrix);

    // bind texture
    m_texture->bind();

    // bind vao
    m_vao.bind();

    // set polygon draw mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  }

  void setColor(const glm::vec4& color) { m_color = color; }
  void rotate(const float radians) {
    m_transformMatrix = glm::rotate(m_transformMatrix, glm::radians(radians),
                                    glm::vec3(0.0f, 0.0f, 1.0f));
  }
  void setScale(const float scale) {
    m_transformMatrix = glm::scale(m_transformMatrix, glm::vec3(scale, scale, scale));
  }
  void setEnableGradient(bool enable) { m_enableBlueGradient = enable ? 1.0f : 0.0f; }
  ~Square() = default;

 private:
  CommonSquareAttributes& attributes = CommonSquareAttributes::get();
  // vertex buffer contains information about each vertex
  VertexBufferObject& m_vbo = attributes.vbo;
  // index buffer defines the order that indices are drawn in position arrays
  VertexBufferObject& m_ibo = attributes.ibo;
  // VAO contains pointers to attributes in the vertex buffer (position, tex coords) and
  // the index buffer
  VertexArrayObject m_vao;
  Texture* m_texture;
  Shader* m_shader;
  glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::mat4 m_transformMatrix = glm::mat4(1.0f);
  float m_enableBlueGradient = 0.0f;
};
#endif