#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <glm/glm.hpp>
#include <reactphysics3d/reactphysics3d.h>

class GameObject {
 public:
  virtual void draw() = 0;

  void setColor(const glm::vec4& color) { m_color = color; }
  void rotate(const float degrees) {
    m_updateModel = true;
    m_rotateMat = m_rotateMat * glm::toMat4(glm::angleAxis(glm::radians(degrees),
                                                           glm::vec3(0.5f, 1.0f, 0.0f)));
  }
  void setRotation(glm::quat quat) {
    m_updateModel = true;
    glm::mat4 rotation = glm::toMat4(quat);
    m_rotateMat = m_rotateMat * rotation;
  }
  void setRotation(const float radians) {
    m_updateModel = true;
    m_rotateMat = glm::toMat4(glm::angleAxis(radians, glm::vec3(1.0f, 1.0f, 1.0f)));
  }
  void setScale(const float scale) {
    m_updateModel = true;
    this->resetScale();
    m_scaleValue = scale;
    m_scaleValue = std::clamp(m_scaleValue, 0.2f, 10.0f);
    m_scaleMat =
        glm::scale(m_scaleMat, glm::vec3(m_scaleValue, m_scaleValue, m_scaleValue));
  }
  void adjustScale(const float scaleAdjustment) {
    m_updateModel = true;
    this->resetScale();
    float scaleFactor = 1.0f + scaleAdjustment;
    m_scaleValue = m_scaleValue * scaleFactor;
    m_scaleValue = std::clamp(m_scaleValue, 0.2f, 10.0f);
    m_scaleMat =
        glm::scale(m_scaleMat, glm::vec3(m_scaleValue, m_scaleValue, m_scaleValue));
  }
  void movePosition(glm::vec3 position) {
    m_updateModel = true;
    m_translateMat = glm::translate(m_translateMat, position);
  }

  reactphysics3d::CollisionBody* getCollisionBody() { return m_body; }

  const glm::mat4& getModel() const { return m_model; }

 protected:
  inline void resetScale() {
    m_scaleMat = glm::scale(
        m_scaleMat, glm::vec3(1 / m_scaleValue, 1 / m_scaleValue, 1 / m_scaleValue));
  }
  inline void setModel() {
    if (m_updateModel) {
      m_model = m_translateMat * m_rotateMat * m_scaleMat;
      glm::mat4 forReact = m_translateMat * m_rotateMat;
      reactphysics3d::Transform transform;
      transform.setFromOpenGL(glm::value_ptr(forReact));
      m_body->setTransform(transform);
      m_collisionShape->setHalfExtents(reactphysics3d::Vector3(
          m_scaleValue / 2.0f, m_scaleValue / 2.0f, m_scaleValue / 2.0f));

      m_updateModel = false;
    }
  }
  float m_scaleValue = 1.0f;
  glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::mat4 m_model = glm::mat4(1.0f);
  // these are multiplied in order top->bottom
  glm::mat4 m_scaleMat = glm::mat4(1.0f);
  glm::mat4 m_translateMat = glm::mat4(1.0f);
  glm::mat4 m_rotateMat = glm::mat4(1.0f);
  bool m_updateModel = true;

  PhysicsProperties* m_physicsProperties = nullptr;
  reactphysics3d::BoxShape* m_collisionShape = nullptr;
  reactphysics3d::CollisionBody* m_body = nullptr;
  reactphysics3d::Collider* m_collider = nullptr;
};

#endif