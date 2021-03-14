#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Command.hpp"

class InputHandler {
 public:
  ~InputHandler() = default;
  InputHandler(GLFWwindow* window) {
    m_window = window;
    bindMinus();
    bindEquals();
  }
  void handleInput() {
    if (glfwGetKey(m_window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
      equals->execute();
    }
    if (glfwGetKey(m_window, GLFW_KEY_MINUS) == GLFW_PRESS) {
      minus->execute();
    }
  }

 private:
  void bindMinus() { minus = (Command*)new ZoomCommand(m_window, -10); }
  void bindEquals() { equals = (Command*)new ZoomCommand(m_window, 10); }
  GLFWwindow* m_window;
  Command* equals;
  Command* minus;
};
#endif