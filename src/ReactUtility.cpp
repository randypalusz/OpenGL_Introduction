#include "ReactUtility.hpp"

reactphysics3d::decimal FirstRayCallback::notifyRaycastHit(
    const reactphysics3d::RaycastInfo& info) {
  return reactphysics3d::decimal(1.0);
}