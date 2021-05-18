#ifndef REACT_UTILITY_HPP
#define REACT_UTILITY_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <reactphysics3d/reactphysics3d.h>

struct PhysicsProperties {
  reactphysics3d::PhysicsWorld* physicsWorld = nullptr;
  reactphysics3d::PhysicsCommon* physicsCommon = nullptr;
};

class FirstRayCallback : public reactphysics3d::RaycastCallback {
 public:
  virtual reactphysics3d::decimal notifyRaycastHit(
      const reactphysics3d::RaycastInfo& info);
  reactphysics3d::CollisionBody* getClosestBody();
  void resetBodies();
  reactphysics3d::CollisionBody* closestBody = nullptr;
  reactphysics3d::decimal closestHitFraction = reactphysics3d::decimal(1.0);
};

#endif
