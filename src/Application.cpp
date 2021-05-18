#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <reactphysics3d/reactphysics3d.h>

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
#include "shapes/GameObject.hpp"
#include "input/Command.hpp"
#include "input/InputHandler.hpp"
#include "Camera.hpp"
#include "Timer.hpp"
#include "shapes/Crosshair.hpp"
#include "OBBIntersection.hpp"
#include "ReactUtility.hpp"
#include "generators/ObjectGenerator.hpp"

auto Application::initApp() -> int {
  int glfwInitRes = glfwInit();
  if (!glfwInitRes) {
    fprintf(stderr, "Unable to initialize GLFW\n");
    return 0;
  }

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_majorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_minorVersion);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

  GLFWwindow* window = nullptr;

  switch (m_fullscreenMode) {
    case FullscreenMode::BORDERLESS:
      m_width = mode->width;
      m_height = mode->height;
      window = glfwCreateWindow(mode->width, mode->height, "InitGL",
                                glfwGetPrimaryMonitor(), nullptr);
      break;
    case FullscreenMode::EXCLUSIVE:
      window = glfwCreateWindow(m_width, m_height, "InitGL", monitor, nullptr);
      break;
    case FullscreenMode::WINDOWED:
      window = glfwCreateWindow(m_width, m_height, "InitGL", nullptr, nullptr);
      break;
    default:
      break;
  }

  if (!window) {
    fprintf(stderr, "Unable to create GLFW window\n");
    glfwTerminate();
    return 0;
  }

  glfwMakeContextCurrent(window);
  // vsync toggle
  glfwSwapInterval(m_vsync ? 1 : 0);

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

  initInternal();

  return 1;
}

void Application::run() {
  Crosshair crosshair{m_window, &m_cameraIndependentShaders.at("Crosshair"),
                      &m_Textures.at("Crosshair")};

  InputHandler handler{m_window, m_camera};

  FirstRayCallback firstRayCallback;

  std::vector<glm::vec3> cubePositions{
      glm::vec3(0.0f, 0.0f, -3.0f),   glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
  std::vector<float> cubeRotations{0.05f, 0.6f,  0.8f,  0.1f,  0.3f,
                                   0.2f,  0.16f, 0.32f, 0.23f, 0.02f};
  std::vector<CubeStruct> cubes;
  for (int i = 0; i < 10; i++) {
    cubes.push_back(
        {cubePositions.at(i),
         (Cube*)ObjectGenerator::create(ObjectType::CUBE, m_shaders.at("Cube"),
                                        m_Textures.at("Cube"), physicsProperties),
         cubeRotations.at(i)});
  }

  for (CubeStruct& cubeStruct : cubes) {
    cubeStruct.cube->movePosition(cubeStruct.position);
    cubeStruct.cube->setEnableGradient(true);
    cubeStruct.cube->setScale(0.8f);
  }

  handler.bindScaleCommands(cubes);

  glm::vec4 colors{0.15f, 1.0f, 1.0f, 1.0f};
  Timer<float> renderTimer{0.0f};
  TimePointTimer logicTimer{};

  // used for fps debugging
  unsigned int numFrames = 0;
  TimePointTimer frameTimer{};

  float increment = 0.02f;
  while (!glfwWindowShouldClose(m_window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    logicTimer.tick(false);

    renderTimer.tick(glfwGetTime());

// output FPS to title
#ifndef NDEBUG
    numFrames++;
    frameTimer.tick(false);
    if (frameTimer.deltaTime >= 1000000) {
      std::string fpsString = "FPS: " + std::to_string(numFrames);
      frameTimer.tick();
      numFrames = 0;
      glfwSetWindowTitle(m_window, fpsString.c_str());
    }
#endif

    // update logic
    logicUpdate(logicTimer, cubes, colors, increment);

    // process input
    handler.handleInput(renderTimer.deltaTime);

    // update camera of each shader
    updateShaderCamera();

    // draw objects
    reactphysics3d::CollisionBody* cb = this->raycast(firstRayCallback);
    if (cb != nullptr) {
      GameObject* obj = (GameObject*)cb->getUserData();
      obj->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    for (CubeStruct& cubeStruct : cubes) {
      cubeStruct.cube->draw();
    }

    // draw crosshair
    crosshair.draw();

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

void Application::updateShaderCamera() {
  for (auto it = m_shaders.begin(); it != m_shaders.end(); it++) {
    Shader& currentShader = it->second;
    m_projection = glm::perspective(glm::radians(m_camera->getZoom()),
                                    (float)m_width / (float)m_height, 0.1f, 100.0f);
    m_view = m_camera->getViewMatrix();

    currentShader.setUniform("u_projection", m_projection);
    currentShader.setUniform("u_view", m_view);
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
      cubeStruct.cube->setColor(colors);
      cubeStruct.cube->rotate(cubeStruct.rotation);
    }
  }
}

reactphysics3d::CollisionBody* Application::raycast(FirstRayCallback& callback) {
  glm::vec3 origin;
  glm::vec3 direction;
  OBBIntersection::screenPosToWorldRay(m_view, m_projection, origin, direction);
  glm::vec3 end = origin + direction * 1000.0f;
  reactphysics3d::Ray ray(reactphysics3d::Vector3(origin.x, origin.y, origin.z),
                          reactphysics3d::Vector3(end.x, end.y, end.z));
  callback.resetBodies();
  physicsProperties.physicsWorld->raycast(ray, &callback);
  return callback.getClosestBody();
}

void Application::initInternal() {
  this->initGL();
  this->initShaders();
  this->initTextures();
  this->initReact();
}

void Application::initReact() {
  reactphysics3d::PhysicsWorld::WorldSettings settings;
  settings.isSleepingEnabled = true;
  settings.gravity = reactphysics3d::Vector3(0, -9.81f, 0);
  physicsProperties.physicsCommon = new reactphysics3d::PhysicsCommon;
  physicsProperties.physicsWorld =
      physicsProperties.physicsCommon->createPhysicsWorld(settings);
}

void Application::initGL() {
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

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // messing with different blend functions
  // glBlendFunc(GL_DST_ALPHA, GL_ONE);

  // Set the clear color to a nice green
  glClearColor(0.15f, 0.6f, 0.4f, 1.0f);
}

void Application::initTextures() {
  m_Textures.emplace("Cube", Texture{"res/wall.jpg"});
  m_Textures.emplace("Crosshair", Texture{"res/crosshair.png"});
}

void Application::initShaders() {
  m_shaders.emplace("Cube", Shader{"res/VertexShader.glsl", "res/FragmentShader.glsl"});
  m_cameraIndependentShaders.emplace("Crosshair",
                                     Shader{"res/crosshair.vs", "res/crosshair.fs"});
}