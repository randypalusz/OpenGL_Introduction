#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <iostream>

enum class Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

struct CameraAttributes {
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;
  float yaw;
  float pitch;
  float speed;
  float mouseSensitivity;
  float zoom;
  CameraAttributes()
      : position{glm::vec3(0.0f, 0.0f, 0.0f)},
        front{glm::vec3(0.0f, 0.0f, -1.0f)},
        up{glm::vec3(0.0f, 1.0f, 0.0f)},
        yaw{-90.0f},
        pitch{0.0f},
        speed{0.05f},
        mouseSensitivity{0.1f},
        zoom{45.0f} {
    worldUp = up;
    right = glm::normalize(glm::cross(front, worldUp));
  }
};

class Camera {
 public:
  Camera(glm::vec3 startPosition) {
    m_attributes.position = startPosition;
    updateVectors();
  }

  Camera(const Camera& camera) = delete;

  glm::mat4 getViewMatrix() {
    return glm::lookAt(m_attributes.position, m_attributes.position + m_attributes.front,
                       m_attributes.up);
  }

  float getZoom() { return m_attributes.zoom; }

  void keyMovement(Movement direction) {
    switch (direction) {
      case Movement::FORWARD:
        m_attributes.position += m_attributes.front * m_attributes.speed;
        break;
      case Movement::BACKWARD:
        m_attributes.position -= m_attributes.front * m_attributes.speed;
        break;
      case Movement::RIGHT:
        m_attributes.position += m_attributes.right * m_attributes.speed;
        break;
      case Movement::LEFT:
        m_attributes.position -= m_attributes.right * m_attributes.speed;
        break;
      case Movement::UP:
        m_attributes.position += m_attributes.up * m_attributes.speed;
        break;
      case Movement::DOWN:
        m_attributes.position -= m_attributes.up * m_attributes.speed;
        break;
    }
  }

  void mouseScroll(float yoffset) {
    m_attributes.zoom -= yoffset;
    // clamp zoom
    m_attributes.zoom = std::clamp(m_attributes.zoom, 1.0f, 90.0f);
  }

  void mouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = GL_TRUE) {
    xoffset *= m_attributes.mouseSensitivity;
    yoffset *= m_attributes.mouseSensitivity;

    m_attributes.yaw += xoffset;
    m_attributes.pitch += yoffset;

    if (constrainPitch) {
      m_attributes.pitch = std::clamp(m_attributes.pitch, -89.0f, 89.0f);
    }

    updateVectors();
  }

 private:
  CameraAttributes m_attributes{};
  void updateVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_attributes.yaw)) * cos(glm::radians(m_attributes.pitch));
    front.y = sin(glm::radians(m_attributes.pitch));
    front.z = sin(glm::radians(m_attributes.yaw)) * cos(glm::radians(m_attributes.pitch));
    m_attributes.front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    m_attributes.right =
        glm::normalize(glm::cross(m_attributes.front, m_attributes.worldUp));
    // normalize the vectors, because their length gets closer to 0
    // the more you look up or down which results in slower movement.
    m_attributes.up = glm::normalize(glm::cross(m_attributes.right, m_attributes.front));
  }
};

#endif