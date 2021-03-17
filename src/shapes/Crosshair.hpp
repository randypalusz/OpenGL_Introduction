#ifndef CROSSHAIR_HPP
#define CROSSHAIR_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vao.hpp"
#include "vbo.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
class CrosshairAttributes {
 public:
  static CrosshairAttributes& get() {
    static CrosshairAttributes instance;
    return instance;
  }
  CrosshairAttributes(CrosshairAttributes const&) = delete;
  void operator=(CrosshairAttributes const&) = delete;
  float* positions;
  unsigned int* indices;
  VertexArrayObject vao{};
  VertexBufferObject ibo{VertexBufferType::IndexBuffer};
  VertexBufferObject vbo{VertexBufferType::VertexBuffer};

 private:
  CrosshairAttributes() {
    positions = new float[20]{
        // positions  // tex coords
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // vertex 1
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  // vertex 2
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // vertex 3
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f   // vertex 4
    };

    indices = new unsigned int[6]{
        0, 1, 2,  // triangle 1
        2, 3, 0   // triangle 2
    };

    ibo.setAttributes(0, sizeof(unsigned int) * 6, indices);
    vbo.setAttributes(0, sizeof(float) * 20, positions);

    vao.setAttributes(vbo, 0, 3, GL_FLOAT, sizeof(float) * 5, 0);

    vao.setAttributes(vbo, 1, 2, GL_FLOAT, sizeof(float) * 5, 3 * sizeof(float));

    vao.setAttributes(ibo, 2, 1, GL_UNSIGNED_INT, sizeof(unsigned int), 0);
  };
};

class Crosshair {
 public:
  Crosshair(GLFWwindow* window, Shader* shader, Texture* texture) {
    glfwGetWindowSize(window, &m_windowWidth, &m_windowHeight);
    m_texture = texture;
    m_shader = shader;
    m_model = glm::scale(m_model, glm::vec3(0.05f, 0.05f, 0.05f));
  }

  void draw() const {
    // enable correct shaders
    m_shader->use();

    // set color uniform for active shader
    m_shader->setUniform("u_Color", m_color);

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
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  }

 private:
  CrosshairAttributes& attributes = CrosshairAttributes::get();
  VertexBufferObject& m_vbo = attributes.vbo;
  VertexBufferObject& m_ibo = attributes.ibo;
  VertexArrayObject& m_vao = attributes.vao;
  Texture* m_texture;
  Shader* m_shader;
  int m_windowWidth;
  int m_windowHeight;
  glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::mat4 m_model = glm::mat4(1.0f);
};

#endif