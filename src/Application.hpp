#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "GLUtility.hpp"
#include "Shader.hpp"
#include "Timer.hpp"
#include "shapes/Cube.hpp"

struct MouseParams {
  bool firstMouse;
  double lastX;
  double lastY;
  MouseParams(int width, int height) : firstMouse{true} {
    lastX = (float)width / 2.0f;
    lastY = (float)height / 2.0f;
  }
};

struct CubeStruct {
  glm::vec3 position;
  Cube cube;
  float rotation;
};

class Application {
 public:
  Application(int width, int height, int majorVersion, int minorVersion)
      : m_width{width},
        m_height{height},
        m_majorVersion{majorVersion},
        m_minorVersion{minorVersion} {
    m_mouseParams = new MouseParams(m_width, m_height);
  }
  ~Application() = default;
  auto init() -> int;
  auto getWindow() -> GLFWwindow* { return m_window; }
  void run();
  void updateShaderCamera(std::vector<Shader>& shaders);
  void logicUpdate(TimePointTimer& timer, std::vector<CubeStruct>& cubes,
                   glm::vec4& colors, float& increment);

 private:
  void scrollCallback(double yoffset);
  void mouseCallback(double xpos, double ypos);
  int m_width;
  int m_height;
  int m_majorVersion;
  int m_minorVersion;
  GLFWwindow* m_window = nullptr;
  Camera* m_camera;
  MouseParams* m_mouseParams;
};

#endif