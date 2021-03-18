#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera.hpp"
#include "shapes/Cube.hpp"

#include <vector>

class Command {
 public:
  virtual ~Command() {}
  virtual void execute(float) = 0;
  virtual void execute() { execute(0.0f); }

 protected:
  Command(){};
};

class ZoomCommand : Command {
 public:
  ZoomCommand(GLFWwindow* window, int zoomLevel) {
    m_zoom = zoomLevel;
    glfwGetWindowSize(window, &m_viewport_width, &m_viewport_height);
  }
  virtual void execute(float deltaTime) {
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

class CameraMovementCommand : Command {
 public:
  CameraMovementCommand(Camera* camera, Movement direction) {
    m_camera = camera;
    m_direction = direction;
  }

  virtual void execute(float deltaTime) { m_camera->keyMovement(m_direction, deltaTime); }

 private:
  Camera* m_camera;
  Movement m_direction;
};

class CloseWindowCommand : Command {
 public:
  CloseWindowCommand(GLFWwindow* window) { m_window = window; };

  virtual void execute(float deltaTime){};
  virtual void execute() { glfwSetWindowShouldClose(m_window, GLFW_TRUE); }

 private:
  GLFWwindow* m_window;
};

class ScaleCubesCommand : Command {
 public:
  ScaleCubesCommand(float scaleFactor, std::vector<CubeStruct>& cubes) : m_cubes(cubes) {
    m_scaleFactor = scaleFactor;
  };

  virtual void execute(float deltaTime) {
    for (CubeStruct& cube : m_cubes) {
      cube.cube.adjustScale(m_scaleFactor * deltaTime);
    }
  };
  virtual void execute() {}

 private:
  std::vector<CubeStruct>& m_cubes;
  float m_scaleFactor;
};
#endif