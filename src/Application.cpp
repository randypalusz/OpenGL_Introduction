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
#include "Camera.hpp"
#include "Timer.hpp"

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

  m_window = window;
  glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));

  // set window resize callback
  glfwSetWindowSizeCallback(m_window, onWindowResize);
  // init mouse callbacks
  glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
    auto appInstance = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    appInstance->mouseCallback(xpos, ypos);
  });
  glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
    auto appInstance = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    appInstance->scrollCallback((float)yoffset);
  });

  // tell GLFW to capture our mouse
  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

  m_camera = new Camera{glm::vec3(0.0f, 0.0f, 3.0f)};

  return 1;
}

void Application::run() {
  std::vector<Shader> shaders{{"res/VertexShader.glsl", "res/FragmentShader.glsl"}};
  Texture texture("res/wall.jpg");

  std::vector<CubeStruct> cubes{
      {glm::vec3(0.0f, 0.0f, -3.0f), {m_window, &shaders.at(0), &texture}, 0.05f},
      {glm::vec3(2.0f, 5.0f, -15.0f), {m_window, &shaders.at(0), &texture}, 0.6f},
      {glm::vec3(-1.5f, -2.2f, -2.5f), {m_window, &shaders.at(0), &texture}, 0.8f},
      {glm::vec3(-3.8f, -2.0f, -12.3f), {m_window, &shaders.at(0), &texture}, 0.1f},
      {glm::vec3(2.4f, -0.4f, -3.5f), {m_window, &shaders.at(0), &texture}, 0.3f},
      {glm::vec3(-1.7f, 3.0f, -7.5f), {m_window, &shaders.at(0), &texture}, 0.2f},
      {glm::vec3(1.3f, -2.0f, -2.5f), {m_window, &shaders.at(0), &texture}, 0.16f},
      {glm::vec3(1.5f, 2.0f, -2.5f), {m_window, &shaders.at(0), &texture}, 0.32f},
      {glm::vec3(1.5f, 0.2f, -1.5f), {m_window, &shaders.at(0), &texture}, 0.23f},
      {glm::vec3(-1.3f, 1.0f, -1.5f), {m_window, &shaders.at(0), &texture}, 0.41f}};

  for (CubeStruct& cubeStruct : cubes) {
    cubeStruct.cube.movePosition(cubeStruct.position);
    cubeStruct.cube.setEnableGradient(true);
    cubeStruct.cube.setScale(0.8f);
  }

  glm::vec4 colors{0.15f, 1.0f, 1.0f, 1.0f};
  Timer<float> renderTimer{0.0f};
  TimePointTimer logicTimer{};
  float increment = 0.02f;
  while (!glfwWindowShouldClose(m_window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    logicTimer.tick(false);

    renderTimer.tick(glfwGetTime());

    // update logic
    logicUpdate(logicTimer, cubes, colors, increment);

    processKeyInput(m_window, *m_camera, renderTimer.deltaTime);

    // update camera of each shader
    updateShaderCamera(shaders);

    for (CubeStruct& cubeStruct : cubes) {
      cubeStruct.cube.draw();
    }

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }

  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Application::scrollCallback(double yoffset) {
  m_camera->mouseScroll((float)yoffset);
}

void Application::mouseCallback(double xpos, double ypos) {
  if (m_mouseParams->firstMouse) {
    m_mouseParams->lastX = xpos;
    m_mouseParams->lastY = ypos;
    m_mouseParams->firstMouse = false;
  }

  float xoffset = xpos - m_mouseParams->lastX;
  float yoffset = m_mouseParams->lastY - ypos;

  m_mouseParams->lastX = xpos;
  m_mouseParams->lastY = ypos;

  m_camera->mouseMovement(xoffset, yoffset);
}

void Application::updateShaderCamera(std::vector<Shader>& shaders) {
  for (Shader& shader : shaders) {
    glm::mat4 projection =
        glm::perspective(glm::radians(m_camera->getZoom()),
                         (float)m_width / (float)m_height, 0.1f, 100.0f);

    glm::mat4 view = m_camera->getViewMatrix();
    shader.setUniformMatrix4fv("u_projection", projection);
    shader.setUniformMatrix4fv("u_view", view);
  }
}

void Application::logicUpdate(TimePointTimer& logicTimer, std::vector<CubeStruct>& cubes,
                              glm::vec4& colors, float& increment) {
  if (logicTimer.deltaTime >= 5000) {
    logicTimer.tick();

    // update a to scroll between 0 and 1.0f, back to 0
    if (colors.r < 0.0f) {
      increment = abs(increment);
    } else if (colors.r > 1.0f) {
      increment = -abs(increment);
    }
    colors.r += increment;

    for (CubeStruct& cubeStruct : cubes) {
      cubeStruct.cube.setColor(colors);
      cubeStruct.cube.rotate(cubeStruct.rotation);
    }
  }
}