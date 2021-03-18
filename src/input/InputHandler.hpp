#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Command.hpp"
#include "Camera.hpp"

class InputHandler {
 public:
  ~InputHandler() = default;
  InputHandler(GLFWwindow* window, Camera* camera) {
    m_window = window;
    m_camera = camera;
    bindCommands();
  }
  void handleInput(float deltaTime) {
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
      w->execute(deltaTime);
    }
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
      a->execute(deltaTime);
    }
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
      s->execute(deltaTime);
    }
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
      d->execute(deltaTime);
    }
    if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
      r->execute(deltaTime);
    }
    if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_PRESS) {
      f->execute(deltaTime);
    }
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      esc->execute();
    }
    if (glfwGetKey(m_window, GLFW_KEY_MINUS) == GLFW_PRESS) {
      minus->execute(deltaTime);
    }
    if (glfwGetKey(m_window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
      plus->execute(deltaTime);
    }
  }

  void bindScaleCommands(std::vector<CubeStruct>& cubes) {
    plus = (Command*)new ScaleCubesCommand(0.8f, cubes);
    minus = (Command*)new ScaleCubesCommand(-0.8f, cubes);
  }

 private:
  void bindCommands() {
    w = (Command*)new CameraMovementCommand(m_camera, Movement::FORWARD);
    a = (Command*)new CameraMovementCommand(m_camera, Movement::LEFT);
    s = (Command*)new CameraMovementCommand(m_camera, Movement::BACKWARD);
    d = (Command*)new CameraMovementCommand(m_camera, Movement::RIGHT);
    r = (Command*)new CameraMovementCommand(m_camera, Movement::UP);
    f = (Command*)new CameraMovementCommand(m_camera, Movement::DOWN);
    esc = (Command*)new CloseWindowCommand(m_window);
  }

  GLFWwindow* m_window;
  Camera* m_camera;
  Command* w;
  Command* a;
  Command* s;
  Command* d;
  Command* r;
  Command* f;
  Command* esc;
  Command* plus;
  Command* minus;
};
#endif