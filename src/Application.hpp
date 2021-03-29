#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>

#include "GLUtility.hpp"
#include "Shader.hpp"
#include "Timer.hpp"
#include "shapes/Cube.hpp"
#include "Texture.hpp"

enum class FullscreenMode { BORDERLESS, WINDOWED, EXCLUSIVE };

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
  Application(int width, int height, int majorVersion, int minorVersion,
              FullscreenMode mode = FullscreenMode::WINDOWED, bool vsync = true)
      : m_width{width},
        m_height{height},
        m_majorVersion{majorVersion},
        m_minorVersion{minorVersion},
        m_fullscreenMode{mode},
        m_vsync{vsync} {
    m_mouseParams = new MouseParams(m_width, m_height);
  }
  ~Application() = default;
  auto initApp() -> int;
  auto getWindow() -> GLFWwindow* { return m_window; }
  void run();

 private:
  void scrollCallback(double yoffset);
  void mouseCallback(double xpos, double ypos);
  void initShaders();
  void initTextures();
  void initGL();
  void initInternal();
  void updateShaderCamera();
  void logicUpdate(TimePointTimer& timer, std::vector<CubeStruct>& cubes,
                   glm::vec4& colors, float& increment);
  int m_width;
  int m_height;
  int m_majorVersion;
  int m_minorVersion;
  FullscreenMode m_fullscreenMode;
  bool m_vsync;
  GLFWwindow* m_window = nullptr;
  Camera* m_camera = new Camera(glm::vec3{0.0f, 0.0f, 3.0f});
  MouseParams* m_mouseParams;
  std::unordered_map<std::string, Shader> m_shaders;
  std::unordered_map<std::string, Shader> m_cameraIndependentShaders;
  std::unordered_map<std::string, Texture> m_Textures;
  glm::mat4 m_projection{1.0f};
  glm::mat4 m_view{1.0f};
};

#endif