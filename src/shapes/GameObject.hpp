#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <glm/glm.hpp>

class GameObject {
 public:
  virtual void draw() = 0;
  virtual void setColor(const glm::vec4& color) = 0;
};

#endif