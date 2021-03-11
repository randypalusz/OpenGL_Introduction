#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdio.h>
#include <iostream>
#include <chrono>
#include <assert.h>

#include "Application.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "vao.hpp"
#include "vbo.hpp"

auto Application::init() -> int {
  int glfwInitRes = glfwInit();
  if (!glfwInitRes) {
    fprintf(stderr, "Unable to initialize GLFW\n");
    return 0;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_majorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_minorVersion);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(m_width, m_height, "InitGL", nullptr, nullptr);
  if (!window) {
    fprintf(stderr, "Unable to create GLFW window\n");
    glfwTerminate();
    return 0;
  }

  glfwMakeContextCurrent(window);

  // set window resize callback
  glfwSetWindowSizeCallback(window, onWindowResize);

  int gladInitRes = gladLoadGL();
  if (!gladInitRes) {
    fprintf(stderr, "Unable to initialize glad\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
  }
#ifdef _WIN32
#ifndef NDEBUG
  // enable debug output after glad is loaded
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
#endif
#endif

  std::cout << glGetString(GL_VERSION) << std::endl;

  m_window = window;
  return 1;
}

void Application::run() {
  // Set the clear color to a nice green
  glClearColor(0.15f, 0.6f, 0.4f, 1.0f);

  float positions[] = {
      // positions  // tex coords
      -0.5f, -0.5f, 0.0f, 0.0f,  // vertex 1
      0.5f,  -0.5f, 1.0f, 0.0f,  // vertex 2
      0.5f,  0.5f,  1.0f, 1.0f,  // vertex 3
      -0.5f, 0.5f,  0.0f, 1.0f   // vertex 4
  };

  float positions2[] = {
      // positions  // tex coords
      -1.0f, -1.0f, 0.0f, 0.0f,  // vertex 1
      0.0f,  -1.0f, 1.0f, 0.0f,  // vertex 2
      -0.5f, -0.5f, 1.0f, 1.0f,  // vertex 3
      -1.0f, 0.0f,  0.0f, 1.0f   // vertex 4
  };

  unsigned int indices[] = {0, 1, 2,   // triangle 1
                            2, 3, 0};  // triangle 2

  // setting up index buffer to define the order that indices are drawn in position arrays
  VertexBufferObject ibo{VertexBufferType::IndexBuffer};
  // (starts at index 0, length = 6 unsigned ints, data is in the indices array)
  ibo.setAttributes(0, sizeof(unsigned int) * 6, indices);

  // vertex array object contains pointers to where different attributes in the VBO reside
  // VAO also contains a pointer to the index buffer that determines index draw order
  VertexArrayObject vao;
  VertexBufferObject vbo{VertexBufferType::VertexBuffer};
  // (bind vertex buffer, choose index 0, 2 elements for this attribute(x,y), float,
  //  distance from this attribute to the next one is the length of 4 floats in bytes,
  //  the first position starts at index 0 in the vertex array)
  vao.setAttributes(vbo, 0, 2, GL_FLOAT, sizeof(float) * 4, 0);
  // (bind vertex buffer, choose index 1, 2 elements for this attribute(x,y), float,
  //  distance from this attribute to the next one is the length of 4 floats in bytes,
  //  the first position starts at index 2 in the index array)
  vao.setAttributes(vbo, 1, 2, GL_FLOAT, sizeof(float) * 4, 2 * sizeof(float));
  // (bind index buffer, choose index 2, 1 element for this attribute (just index), uint,
  //  distance from this attribute to the next one is the length of one uint in bytes,
  //  the first position starts at index 0 in the index array)
  vao.setAttributes(ibo, 2, 1, GL_UNSIGNED_INT, sizeof(unsigned int), 0);
  // (starts at index 0, length = 16 floats, data is in the positions array)
  vbo.setAttributes(0, sizeof(float) * 16, positions);

  // using this to demonstrate a second rectangle
  VertexArrayObject vao2;
  VertexBufferObject vbo2{VertexBufferType::VertexBuffer};
  vao2.setAttributes(vbo2, 0, 2, GL_FLOAT, sizeof(float) * 4, 0);
  vao2.setAttributes(vbo2, 1, 2, GL_FLOAT, sizeof(float) * 4, sizeof(float) * 2);
  vao2.setAttributes(ibo, 2, 1, GL_UNSIGNED_INT, sizeof(unsigned int), 0);
  vbo2.setAttributes(0, sizeof(float) * 16, positions2);
  // rect2 end

  Shader shader("res/VertexShader.glsl", "res/FragmentShader.glsl");
  Texture texture("res/wall.jpg");

  // clear bindings
  // glBindVertexArray(0);
  // glUseProgram(0);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  auto t0 = std::chrono::high_resolution_clock::now();
  auto t1 = std::chrono::high_resolution_clock::now();
  float r = 0.15f;
  float g = 0.6f;
  float b = 0.4f;
  float a = 1.0f;
  float inc = 0.05f;
  while (!glfwWindowShouldClose(m_window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    t1 = std::chrono::high_resolution_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() >= 50) {
      // reset the start point
      t0 = std::chrono::high_resolution_clock::now();
      // update a to scroll between 0 and 1.0f, back to 0
      if (r < 0.0f) {
        inc = 0.05f;
      } else if (r > 1.0f) {
        inc = -0.05f;
      }
      r += inc;
    }

    // enable correct shaders
    shader.use();

    // set color uniform for active shader
    shader.setUniform4f("u_Color", glm::vec4(r, g, b, a));

    // set gradient uniform for active shader
    shader.setUniform1f("u_enableBlueGradient", 1.0f);

    // bind texture
    texture.bind();

    // bind vao
    vao.bind();

    // set polygon draw mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    shader.setUniform4f("u_Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    shader.setUniform1f("u_enableBlueGradient", 0.0f);
    vao2.bind();
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }

  glfwDestroyWindow(m_window);
  glfwTerminate();
}