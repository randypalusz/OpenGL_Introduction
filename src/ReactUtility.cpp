#include "ReactUtility.hpp"
#include <iostream>

reactphysics3d::decimal FirstRayCallback::notifyRaycastHit(
    const reactphysics3d::RaycastInfo& info) {
  lastCollided = info.body;
  // Display the world hit point coordinates
  std::cout << "Hit point : " << info.worldPoint.x << info.worldPoint.y
            << info.worldPoint.z << std::endl;

  return reactphysics3d::decimal(0.0);
}