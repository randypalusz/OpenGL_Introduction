#ifndef CUBE_HPP
#define CUBE_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vao.hpp"
#include "vbo.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "vbLayout.hpp"

// Singleton that will hold common square attributes (index buffer, vbo)
class CubeAttributes {
 public:
  static CubeAttributes& get() {
    static CubeAttributes instance;
    return instance;
  }
  CubeAttributes(CubeAttributes const&) = delete;
  void operator=(CubeAttributes const&) = delete;
  float* positions;
  unsigned int* indices;
  VertexArrayObject vao{};
  VertexBufferObject ibo{VertexBufferType::IndexBuffer};
  VertexBufferObject vbo{VertexBufferType::VertexBuffer};

 private:
  CubeAttributes() {
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
    vbo.setAttributes(0, sizeof(float) * 180, positions);

    VertexBufferLayout layout{};
    layout.push<float>(3);
    layout.push<float>(2);

    vao.pushLayout(vbo, layout);
  };
};

class Cube {
 public:
  Cube(GLFWwindow* window, Shader* shader, Texture* texture) {
    glfwGetWindowSize(window, &m_windowWidth, &m_windowHeight);
    m_texture = texture;
    m_shader = shader;
  }

  void draw() const {
    // enable correct shaders
    m_shader->use();

    // set color uniform for active shader
    m_shader->setUniform("u_Color", m_color);

    // set gradient uniform for active shader
    m_shader->setUniform("u_enableBlueGradient", m_enableBlueGradient);

    // set model matrix
    m_shader->setUniform("u_model", m_model);

    // bind texture
    m_texture->bind();

    // bind vao
    m_vao.bind();

    // set polygon draw mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // draw
    // glDrawElements if index buffer
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  void setColor(const glm::vec4& color) { m_color = color; }
  void rotate(const float radians) {
    m_model = glm::rotate(m_model, glm::radians(radians), glm::vec3(0.5f, 1.0f, 0.0f));
  }
  void setScale(const float scale) {
    this->resetScale();
    m_scale = scale;
    m_scale = std::clamp(m_scale, 0.2f, 10.0f);
    m_model = glm::scale(m_model, glm::vec3(m_scale, m_scale, m_scale));
  }
  void adjustScale(const float scaleAdjustment) {
    this->resetScale();
    float scaleFactor = 1.0f + scaleAdjustment;
    m_scale = m_scale * scaleFactor;
    m_scale = std::clamp(m_scale, 0.2f, 10.0f);
    m_model = glm::scale(m_model, glm::vec3(m_scale, m_scale, m_scale));
  }
  void movePosition(glm::vec3 position) { m_model = glm::translate(m_model, position); }

  void setEnableGradient(bool enable) { m_enableBlueGradient = enable ? 1.0f : 0.0f; }

  ~Cube() = default;

 private:
  inline void resetScale() {
    m_model = glm::scale(m_model, glm::vec3(1 / m_scale, 1 / m_scale, 1 / m_scale));
  }
  CubeAttributes& attributes = CubeAttributes::get();
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
  float m_scale = 1.0f;
  glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::mat4 m_model = glm::mat4(1.0f);
  float m_enableBlueGradient = 0.0f;
};
#endif