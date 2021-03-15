#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdio.h>
#include <iostream>
#include <chrono>
#include <assert.h>
#include <vector>

#include "Application.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include "shapes/Cube.hpp"
#include "input/Command.hpp"
#include "input/InputHandler.hpp"

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

  // perform depth test for 3D
  glEnable(GL_DEPTH_TEST);

  // Set the clear color to a nice green
  glClearColor(0.15f, 0.6f, 0.4f, 1.0f);

  m_window = window;
  return 1;
}

void Application::run() {
  struct CubeStruct {
    glm::vec3 position;
    Cube cube;
    float rotation;
  };
  InputHandler inputHandler{m_window};

  Shader shader("res/VertexShader.glsl", "res/FragmentShader.glsl");
  Texture texture("res/wall.jpg");

  std::vector<CubeStruct> cubes{
      {glm::vec3(0.0f, 0.0f, -3.0f), {m_window, &shader, &texture}, 0.05f},
      {glm::vec3(2.0f, 5.0f, -15.0f), {m_window, &shader, &texture}, 0.6f},
      {glm::vec3(-1.5f, -2.2f, -2.5f), {m_window, &shader, &texture}, 0.8f},
      {glm::vec3(-3.8f, -2.0f, -12.3f), {m_window, &shader, &texture}, 0.1f},
      {glm::vec3(2.4f, -0.4f, -3.5f), {m_window, &shader, &texture}, 0.3f},
      {glm::vec3(-1.7f, 3.0f, -7.5f), {m_window, &shader, &texture}, 0.2f},
      {glm::vec3(1.3f, -2.0f, -2.5f), {m_window, &shader, &texture}, 0.16f},
      {glm::vec3(1.5f, 2.0f, -2.5f), {m_window, &shader, &texture}, 0.32f},
      {glm::vec3(1.5f, 0.2f, -1.5f), {m_window, &shader, &texture}, 0.23f},
      {glm::vec3(-1.3f, 1.0f, -1.5f), {m_window, &shader, &texture}, 0.41f}};

  for (CubeStruct& cubeStruct : cubes) {
    cubeStruct.cube.movePosition(cubeStruct.position);
    cubeStruct.cube.setEnableGradient(true);
    cubeStruct.cube.setScale(0.8f);
  }

  auto t0 = std::chrono::high_resolution_clock::now();
  auto t1 = std::chrono::high_resolution_clock::now();
  float r = 0.15f;
  float g = 0.6f;
  float b = 0.4f;
  float a = 1.0f;
  float inc = 0.05f;
  while (!glfwWindowShouldClose(m_window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    t1 = std::chrono::high_resolution_clock::now();

    // ~144 fps (frametime ~13.333 ms)
    if (std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() >= 6944) {
      inputHandler.handleInput();
      // reset the start point
      t0 = std::chrono::high_resolution_clock::now();
      // update a to scroll between 0 and 1.0f, back to 0
      if (r < 0.0f) {
        inc = 0.05f;
      } else if (r > 1.0f) {
        inc = -0.05f;
      }
      r += inc;
      for (CubeStruct& cubeStruct : cubes) {
        cubeStruct.cube.setColor(glm::vec4(r, 1.0f, 0.0f, 1.0f));
        cubeStruct.cube.rotate(cubeStruct.rotation);
      }
    }
    for (CubeStruct& cubeStruct : cubes) {
      cubeStruct.cube.draw();
    }

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }

  glfwDestroyWindow(m_window);
  glfwTerminate();
}