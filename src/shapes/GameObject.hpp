#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <glm/glm.hpp>
#include <reactphysics3d/reactphysics3d.h>

class GameObject {
 public:
  virtual void draw() = 0;
  virtual void setColor(const glm::vec4& color) = 0;
  virtual reactphysics3d::CollisionBody* getCollisionBody() = 0;
};

#endif