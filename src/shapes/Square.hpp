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
  VertexArrayObject vao{};
  VertexBufferObject ibo{VertexBufferType::IndexBuffer};
  VertexBufferObject vbo{VertexBufferType::VertexBuffer};

 private:
  CommonSquareAttributes() {
    // positions = new float[20]{
    //     // positions  // tex coords
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // vertex 1
    //     0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  // vertex 2
    //     0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // vertex 3
    //     -0.5f, 0.5f,  0.0f, 0.0f, 1.0f   // vertex 4
    // };
    positions = new float[180]{
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //
        0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,  //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,  //
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //
        -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,  //
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,  //
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f   //
    };
    // indices = new unsigned int[6]{
    //     0, 1, 2,  // triangle 1
    //     2, 3, 0   // triangle 2
    // };
    // (starts at index 0, length = 6 unsigned ints, data is in the indices array)
    // ibo.setAttributes(0, sizeof(unsigned int) * 6, indices);
    // (starts at index 0, length = 20 floats, data is in the positions array)
    vbo.setAttributes(0, sizeof(float) * 180, positions);

    // (vertex coordinates)
    // (bind vertex buffer, choose index 0, 3 elements for this attribute(x,y,z), float,
    //  distance from this attribute to the next one is the length of 5 floats in bytes,
    //  the first position starts at index 0 in the vertex array)
    vao.setAttributes(vbo, 0, 3, GL_FLOAT, sizeof(float) * 5, 0);

    // (texture coordinates)
    // (bind vertex buffer, choose index 1, 2 elements for this attribute(x,y), float,
    //  distance from this attribute to the next one is the length of 5 floats in bytes,
    //  the first position starts at index 3 in the index array)
    vao.setAttributes(vbo, 1, 2, GL_FLOAT, sizeof(float) * 5, 3 * sizeof(float));

    // (bind index buffer, choose index 2, 1 element for this attribute (just index),
    // uint,
    //  distance from this attribute to the next one is the length of one uint in bytes,
    //  the first position starts at index 0 in the index array)
    // m_vao.setAttributes(m_ibo, 2, 1, GL_UNSIGNED_INT, sizeof(unsigned int), 0);
  };
};

class Square {
 public:
  Square(GLFWwindow* window, Shader* shader, Texture* texture) {
    glfwGetWindowSize(window, &m_windowWidth, &m_windowHeight);
    m_texture = texture;
    m_shader = shader;

    initMatrices();
  }
  void draw() const {
    // enable correct shaders
    m_shader->use();

    // set color uniform for active shader
    m_shader->setUniform4f("u_Color", m_color);

    // set gradient uniform for active shader
    m_shader->setUniform1f("u_enableBlueGradient", m_enableBlueGradient);

    // set model matrix
    m_shader->setUniformMatrix4fv("u_model", m_model);

    // set view matrix
    m_shader->setUniformMatrix4fv("u_view", m_view);

    // bind texture
    m_texture->bind();

    // bind vao
    m_vao.bind();

    // set polygon draw mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // draw
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  void initMatrices() {
    m_projection = glm::perspective(
        glm::radians(45.0f), (float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.0f);
    m_view = glm::translate(m_view, glm::vec3(0.0f, 0.0f, -3.0f));
    m_shader->use();
    m_shader->setUniformMatrix4fv("u_projection", m_projection);
  }

  void setColor(const glm::vec4& color) { m_color = color; }
  void rotate(const float radians) {
    m_model = glm::rotate(m_model, glm::radians(radians), glm::vec3(0.5f, 1.0f, 0.0f));
  }
  void setScale(const float scale) {
    m_model = glm::scale(m_model, glm::vec3(scale, scale, scale));
  }
  void movePosition(glm::vec3 position) { m_model = glm::translate(m_model, position); }

  void setEnableGradient(bool enable) { m_enableBlueGradient = enable ? 1.0f : 0.0f; }

  ~Square() = default;

 private:
  CommonSquareAttributes& attributes = CommonSquareAttributes::get();
  // vertex buffer contains information about each vertex
  VertexBufferObject& m_vbo = attributes.vbo;
  // index buffer defines the order that indices are drawn in position arrays
  // VertexBufferObject& m_ibo = attributes.ibo;
  // VAO contains pointers to attributes in the vertex buffer (position, tex coords) and
  // the index buffer
  VertexArrayObject& m_vao = attributes.vao;
  Texture* m_texture;
  Shader* m_shader;
  int m_windowWidth;
  int m_windowHeight;
  glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::mat4 m_model = glm::mat4(1.0f);
  glm::mat4 m_projection = glm::mat4(1.0f);
  glm::mat4 m_view = glm::mat4(1.0f);
  float m_enableBlueGradient = 0.0f;
};
#endif