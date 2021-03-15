#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLUtility.hpp"

class Application {
 public:
  Application(int width, int height, int majorVersion, int minorVersion)
      : m_width{width},
        m_height{height},
        m_majorVersion{majorVersion},
        m_minorVersion{minorVersion} {}
  ~Application() = default;
  auto init() -> int;
  auto getWindow() -> GLFWwindow* { return m_window; }
  void run();

 private:
  int m_width;
  int m_height;
  int m_majorVersion;
  int m_minorVersion;
  GLFWwindow* m_window = nullptr;
};

#endif