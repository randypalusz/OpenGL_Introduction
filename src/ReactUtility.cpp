#include "ReactUtility.hpp"
#include <iostream>
#include <shapes/Cube.hpp>
#include <glm/glm.hpp>

reactphysics3d::decimal FirstRayCallback::notifyRaycastHit(
    const reactphysics3d::RaycastInfo& info) {
  if (closestBody == nullptr) {
    closestBody = info.body;
    closestHitFraction = info.hitFraction;
  } else {
    if (closestHitFraction < info.hitFraction) {
      closestBody = info.body;
      closestHitFraction = info.hitFraction;
    }
  }

  return reactphysics3d::decimal(1.0);
}

void FirstRayCallback::resetBodies() { closestBody = nullptr; }
reactphysics3d::CollisionBody* FirstRayCallback::getClosestBody() { return closestBody; }