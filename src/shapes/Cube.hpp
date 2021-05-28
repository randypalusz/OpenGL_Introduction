#ifndef CUBE_HPP
#define CUBE_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <reactphysics3d/reactphysics3d.h>
#include <algorithm>
#include <vector>
#include "vao.hpp"
#include "vbo.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "vbLayout.hpp"
#include "ReactUtility.hpp"
#include "GameObject.hpp"

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

class Cube : public GameObject {
 public:
  Cube(Shader* shader, Texture* texture, PhysicsProperties* physicsProperties) {
    m_physicsProperties = physicsProperties;
    m_texture = texture;
    m_shader = shader;
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

  void setEnableGradient(bool enable) { m_enableBlueGradient = enable ? 1.0f : 0.0f; }

  ~Cube() = default;

 private:
  void initReact() {
    const reactphysics3d::Vector3 halfExtents(0.5f, 0.5f, 0.5f);
    m_collisionShape = m_physicsProperties->physicsCommon->createBoxShape(halfExtents);
    reactphysics3d::Vector3 position(0.0f, 0.0f, 0.0f);
    reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
    reactphysics3d::Transform transform(position, orientation);
    m_body = m_physicsProperties->physicsWorld->createCollisionBody(transform);
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
  float m_enableBlueGradient = 0.0f;
};
#endif