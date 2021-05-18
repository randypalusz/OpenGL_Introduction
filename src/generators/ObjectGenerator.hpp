#ifndef SHAPE_GEN_HPP
#define SHAPE_GEN_HPP

#include "shapes/GameObject.hpp"
#include "shapes/Cube.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "ReactUtility.hpp"
#include <memory>
#include <iostream>

enum class ObjectType { CUBE };

// TODO: This is the replacement for calling setUserData in the Application class, objects
// should only be created through this API in the future
// also evaluate whether this is the best way of doing things, as moving towards heap
// allocated objects

// TODO: make shape constructors private so that they can only be called here
class ObjectGenerator {
 public:
  static GameObject* create(ObjectType type, Shader& shader, Texture& texture,
                            PhysicsProperties& props) {
    GameObject* obj = nullptr;
    switch (type) {
      case ObjectType::CUBE:
        obj = new Cube(&shader, &texture, props);
        break;
      default:
        return nullptr;
    }
    obj->getCollisionBody()->setUserData(obj);
    return obj;
  }

 private:
  ObjectGenerator();
};

#endif