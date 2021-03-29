#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <btBulletDynamicsCommon.h>

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
#include "shapes/Crosshair.hpp"
#include "OBBIntersection.hpp"

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
  // TODO: add input parameter to determine vsync or not
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

  Cube cube{m_window, &m_shaders.at("Cube"), &m_Textures.at("Cube")};

  std::vector<CubeStruct> cubes{{glm::vec3(0.0f, 0.0f, -3.0f), cube, 0.05f},
                                {glm::vec3(2.0f, 5.0f, -15.0f), cube, 0.6f},
                                {glm::vec3(-1.5f, -2.2f, -2.5f), cube, 0.8f},
                                {glm::vec3(-3.8f, -2.0f, -12.3f), cube, 0.1f},
                                {glm::vec3(2.4f, -0.4f, -3.5f), cube, 0.3f},
                                {glm::vec3(-1.7f, 3.0f, -7.5f), cube, 0.2f},
                                {glm::vec3(1.3f, -2.0f, -2.5f), cube, 0.16f},
                                {glm::vec3(1.5f, 2.0f, -2.5f), cube, 0.32f},
                                {glm::vec3(1.5f, 0.2f, -1.5f), cube, 0.23f},
                                {glm::vec3(-1.3f, 1.0f, -1.5f), cube, 0.02f}};

  for (CubeStruct& cubeStruct : cubes) {
    cubeStruct.cube.movePosition(cubeStruct.position);
    cubeStruct.cube.setEnableGradient(true);
    // cubeStruct.cube.setScale(0.8f);
  }

  // TODO: this is a hack - ensure that position move is done first, then set rotation
  // TODO: move rigid body information into cube class
  std::vector<glm::quat> rotations;
  std::vector<btRigidBody*> rigidbodies;

  // In this example, all monkeys will use the same collision shape :
  // A box of 2m*2m*2m (1.0 is the half-extent !)
  // TODO: make the bounding box dynamic according to the shape/size
  btCollisionShape* boxCollisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
  for (int i = 0; i < 10; i++) {
    rotations.push_back(
        glm::normalize(glm::quat(glm::vec3(rand() % 360, rand() % 360, rand() % 360))));
    CubeStruct& cube = cubes.at(i);
    cube.cube.setRotation(rotations.at(i));

    btDefaultMotionState* motionstate = new btDefaultMotionState(
        btTransform(btQuaternion(rotations.at(i).x, rotations.at(i).y, rotations.at(i).z,
                                 rotations.at(i).w),
                    btVector3(cube.position.x, cube.position.y, cube.position.z)));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        0,  // mass, in kg. 0 -> Static object, will never move.
        motionstate,
        boxCollisionShape,  // collision shape of body
        btVector3(0, 0, 0)  // local inertia
    );
    btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);

    rigidbodies.push_back(rigidBody);
    m_dynamicsWorld->addRigidBody(rigidBody);

    rigidBody->setUserPointer((CubeStruct*)&cubes.at(i));
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

    // draw cubes
    glm::vec3 origin;
    glm::vec3 direction;
    OBBIntersection::screenPosToWorldRay(m_view, m_projection, origin, direction);
    glm::vec3 end = origin + direction * 1000.0f;
    btCollisionWorld::ClosestRayResultCallback RayCallback(
        btVector3(origin.x, origin.y, origin.z), btVector3(end.x, end.y, end.z));
    // TODO: investigate why end/origin need to be switched here to get the first ray
    m_dynamicsWorld->rayTest(btVector3(end.x, end.y, end.z),
                             btVector3(origin.x, origin.y, origin.z), RayCallback);
    // std::cout << glm::to_string(end) << std::endl;
    if (RayCallback.hasHit()) {
      CubeStruct* p = (CubeStruct*)RayCallback.m_collisionObject->getUserPointer();
      p->cube.setColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
      // std::cout << "Ray collides with cube: " << std::endl;
    } else {
      // std::cout << "No collision" << std::endl;
    }
    for (CubeStruct& cubeStruct : cubes) {
      cubeStruct.cube.draw();
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
      cubeStruct.cube.setColor(colors);
      // cubeStruct.cube.rotate(cubeStruct.rotation);
    }
  }
}

void Application::initInternal() {
  this->initGL();
  this->initShaders();
  this->initTextures();
  this->initBullet();
}

void Application::initBullet() {
  // Initialize Bullet. This strictly follows
  // http://bulletphysics.org/mediawiki-1.5.8/index.php/Hello_World, even though we won't
  // use most of this stuff.

  // Build the broadphase
  btBroadphaseInterface* broadphase = new btDbvtBroadphase();

  // Set up the collision configuration and dispatcher
  btDefaultCollisionConfiguration* collisionConfiguration =
      new btDefaultCollisionConfiguration();
  btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

  // The actual physics solver
  btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

  // The world.
  m_dynamicsWorld =
      new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
  m_dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
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