#ifndef CUBE_HPP
#define CUBE_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <btBulletDynamicsCommon.h>
#include <reactphysics3d/reactphysics3d.h>
#include <algorithm>
#include <vector>
#include "vao.hpp"
#include "vbo.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "vbLayout.hpp"
#include "ReactUtility.hpp"

// Singleton that will hold common square attributes (index buffer, vbo)
class CubeAttributes {
 public:
  static CubeAttributes& get() {
    static CubeAttributes instance;
    return instance;
  }
  CubeAttributes(CubeAttributes const&) = delete;
  void operator=(CubeAttributes const&) = delete;
  std::vector<float> positions;
  unsigned int* indices;
  VertexArrayObject vao{};
  VertexBufferObject ibo{VertexBufferType::IndexBuffer};
  VertexBufferObject vbo{VertexBufferType::VertexBuffer};

 private:
  CubeAttributes() {
    positions = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //
        0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,  //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,  //
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //
        -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,  //
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,  //
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f   //
    };
    vbo.setData(positions);

    VertexBufferLayout layout{};
    layout.push<float>(3);
    layout.push<float>(2);

    vao.pushLayout(vbo, layout);
  };
};

class Cube {
 public:
  Cube(Shader* shader, Texture* texture, PhysicsProperties& physicsProperties)
      : m_physicsProperties(physicsProperties) {
    m_texture = texture;
    m_shader = shader;
    // m_dynamicsWorld = dw;
    // this->initBullet();
    this->initReact();
  }

  void draw() {
    // enable correct shaders
    m_shader->use();

    // set color uniform for active shader
    m_shader->setUniform("u_Color", m_color);

    // set gradient uniform for active shader
    m_shader->setUniform("u_enableBlueGradient", m_enableBlueGradient);

    // update model matrix;
    this->setModel();

    // set model matrix
    m_shader->setUniform("u_model", m_model);

    // bind texture
    m_texture->bind();

    // bind vao
    m_vao.bind();

    // set polygon draw mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // draw
    // glDrawElements if index buffer
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

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

  void setEnableGradient(bool enable) { m_enableBlueGradient = enable ? 1.0f : 0.0f; }

  // btRigidBody* getRigidBody() { return m_rigidBody; }
  reactphysics3d::CollisionBody* getCollisionBody() { return m_body; }

  const glm::mat4& getModel() const { return m_model; }

  ~Cube() = default;

 private:
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
  void initReact() {
    const reactphysics3d::Vector3 halfExtents(0.5f, 0.5f, 0.5f);
    m_collisionShape = m_physicsProperties.physicsCommon->createBoxShape(halfExtents);
    reactphysics3d::Vector3 position(0.0f, 0.0f, 0.0f);
    reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
    reactphysics3d::Transform transform(position, orientation);
    m_body = m_physicsProperties.physicsWorld->createCollisionBody(transform);
    m_collider =
        m_body->addCollider(m_collisionShape, reactphysics3d::Transform::identity());
  }
  CubeAttributes& attributes = CubeAttributes::get();
  // vertex buffer contains information about each vertex
  VertexBufferObject& m_vbo = attributes.vbo;
  // index buffer defines the order that indices are drawn in position arrays
  // VertexBufferObject& m_ibo = attributes.ibo;
  // VAO contains pointers to attributes in the vertex buffer (position, tex coords) and
  // the index buffer
  VertexArrayObject& m_vao = attributes.vao;
  Texture* m_texture;
  Shader* m_shader;
  float m_scaleValue = 1.0f;
  glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::mat4 m_model = glm::mat4(1.0f);
  // these are multiplied in order top->bottom
  glm::mat4 m_scaleMat = glm::mat4(1.0f);
  glm::mat4 m_translateMat = glm::mat4(1.0f);
  glm::mat4 m_rotateMat = glm::mat4(1.0f);
  float m_enableBlueGradient = 0.0f;

  PhysicsProperties& m_physicsProperties;
  reactphysics3d::BoxShape* m_collisionShape = nullptr;
  reactphysics3d::CollisionBody* m_body = nullptr;
  reactphysics3d::Collider* m_collider = nullptr;
  // TODO: find a more elegant way of setting this
  bool m_updateModel = true;
};
#endif