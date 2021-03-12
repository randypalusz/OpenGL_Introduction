#ifndef SQUARE_HPP
#define SQUARE_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
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
    ibo.setAttributes(0, sizeof(unsigned int) * 6, indices);
    vbo.setAttributes(0, sizeof(float) * 16, positions);
  };
};

class Square {
 public:
  Square(Shader* shader, Texture* texture) {
    m_texture = texture;
    m_shader = shader;
    m_vao.setAttributes(m_vbo, 0, 2, GL_FLOAT, sizeof(float) * 4, 0);
    m_vao.setAttributes(m_vbo, 1, 2, GL_FLOAT, sizeof(float) * 4, 2 * sizeof(float));
    m_vao.setAttributes(m_ibo, 2, 1, GL_UNSIGNED_INT, sizeof(unsigned int), 0);
  }
  void draw() {
    // enable correct shaders
    m_shader->use();

    // set color uniform for active shader
    m_shader->setUniform4f("u_Color", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

    // set gradient uniform for active shader
    m_shader->setUniform1f("u_enableBlueGradient", 1.0f);

    // bind texture
    m_texture->bind();

    // bind vao
    m_vao.bind();

    // set polygon draw mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  }
  ~Square() = default;

 private:
  CommonSquareAttributes& attributes = CommonSquareAttributes::get();
  VertexArrayObject m_vao;
  VertexBufferObject& m_vbo = attributes.vbo;
  VertexBufferObject& m_ibo = attributes.ibo;
  Texture* m_texture;
  Shader* m_shader;
};
#endif