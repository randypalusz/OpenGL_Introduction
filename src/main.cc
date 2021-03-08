#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

#include <GLUtility.hpp>
GLFWwindow* init() {
  int glfwInitRes = glfwInit();
  if (!glfwInitRes) {
    fprintf(stderr, "Unable to initialize GLFW\n");
    return nullptr;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(1280, 720, "InitGL", nullptr, nullptr);
  if (!window) {
    fprintf(stderr, "Unable to create GLFW window\n");
    glfwTerminate();
    return nullptr;
  }

  glfwMakeContextCurrent(window);

  int gladInitRes = gladLoadGL();
  if (!gladInitRes) {
    fprintf(stderr, "Unable to initialize glad\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    return nullptr;
  }
#ifdef _WIN32
  // enable debug output after glad is loaded
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
  // end enable debug output
#endif

  std::cout << glGetString(GL_VERSION) << std::endl;

  return window;
}

int main() {
  // During init, enable debug output
  GLFWwindow* window = init();
  if (!window) {
    return 0;
  }

  // Set the clear color to a nice green
  // glClearColor(0.15f, 0.6f, 0.4f, 1.0f);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  float positions[] = {
      -0.5f, -0.5f,  // vertex 1
      0.0f,  0.5f,   // vertex 2
      0.5f,  -0.5f   // vertex 3
  };

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}