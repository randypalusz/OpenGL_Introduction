#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Command {
 public:
  virtual ~Command() {}
  virtual void execute() = 0;

 protected:
  Command(){};
};

class ZoomCommand : Command {
 public:
  ZoomCommand(GLFWwindow* window, int zoomLevel) {
    m_zoom = zoomLevel;
    glfwGetWindowSize(window, &m_viewport_width, &m_viewport_height);
  }
  virtual void execute() {
    m_viewport_height += m_zoom;
    m_viewport_width += m_zoom;

    glViewport(0, 0, m_viewport_width, m_viewport_height);
  }

 private:
  int m_zoom;
  int m_viewport_height;
  int m_viewport_width;
  GLFWwindow* m_window;
};

#endif