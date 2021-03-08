#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

#include "Program.hpp"

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
      0.0f,  0.5f,   // vertex 2
      0.5f,  -0.5f   // vertex 3
  };

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);

  // glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  while (!glfwWindowShouldClose(m_window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }

  glfwDestroyWindow(m_window);
  glfwTerminate();
}