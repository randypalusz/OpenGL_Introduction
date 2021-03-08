#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>
#include <chrono>
#include <assert.h>

#include "Program.hpp"
#include "Shader.hpp"

auto Program::init() -> int {
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

  int gladInitRes = gladLoadGL();
  if (!gladInitRes) {
    fprintf(stderr, "Unable to initialize glad\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
  }
#ifdef _WIN32
  // enable debug output after glad is loaded
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
  // end enable debug output
#endif

  std::cout << glGetString(GL_VERSION) << std::endl;

  m_window = window;
  return 1;
}

void Program::run() {
  // Set the clear color to a nice green
  glClearColor(0.15f, 0.6f, 0.4f, 1.0f);

  float positions[] = {
      -0.5f, -0.5f,  // vertex 1
      0.5f,  -0.5f,  // vertex 2
      0.5f,  0.5f,   // vertex 3
      -0.5f, 0.5f    // vertex 4
  };

  unsigned int indices[] = {0, 1, 2,   // triangle 1
                            2, 3, 0};  // triangle 2

  unsigned int vertexArrayObject;
  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);

  unsigned int vertexBufferObject;
  glGenBuffers(1, &vertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  unsigned int indexBufferObject;
  glGenBuffers(1, &indexBufferObject);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 6, indices, GL_STATIC_DRAW);

  unsigned int program =
      ShaderUtils::CreateShader("res/VertexShader.glsl", "res/FragmentShader.glsl");
  glUseProgram(program);

  int colorUniformLocation = glGetUniformLocation(program, "u_Color");
  assert(colorUniformLocation != -1);

  // clear bindings
  glBindVertexArray(0);
  glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

    // activate shaders
    glUseProgram(program);

    // set uniform for active shader
    glUniform4f(colorUniformLocation, r, g, b, a);

    // bind index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

    // bind vao
    glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }

  glfwDestroyWindow(m_window);
  glfwTerminate();
}