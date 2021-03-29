#include "OBBIntersection.hpp"

#include <assert.h>

bool OBBIntersection::isIntersection(const glm::vec3& origin,
                                     const glm::vec3& normalizedDirection,
                                     const glm::vec3& aabbMin, const glm::vec3& aabbMax,
                                     const glm::mat4& model,
                                     float& intersectionDistance) {
  float tMin = 0.0f;
  float tMax = 100000.0f;

  glm::vec3 BBworldPosition{model[3].x, model[3].y, model[3].z};
  glm::vec3 delta = BBworldPosition - origin;

  auto testIntersectionOnAxis = [&](Axis axis) {
    int modelIndex = -1;
    float aabbMaxForAxis;
    float aabbMinForAxis;
    switch (axis) {
      case Axis::X:
        modelIndex = 0;
        aabbMaxForAxis = aabbMax.x;
        aabbMinForAxis = aabbMin.x;
        break;
      case Axis::Y:
        modelIndex = 1;
        aabbMaxForAxis = aabbMax.y;
        aabbMinForAxis = aabbMin.y;
        break;
      case Axis::Z:
        modelIndex = 2;
        aabbMaxForAxis = aabbMax.z;
        aabbMinForAxis = aabbMin.z;
        break;
    }
    assert(modelIndex != -1);

    glm::vec4 column{model[modelIndex]};
    glm::vec3 axisValues{column.x, column.y, column.z};
    float e = glm::dot(axisValues, delta);
    float f = glm::dot(normalizedDirection, axisValues);

    if (fabs(f) > 0.001f) {
      float t1 = (e + aabbMinForAxis) / f;  // Intersection with the "left" plane
      float t2 = (e + aabbMaxForAxis) / f;  // Intersection with the "right" plane
      // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

      // We want t1 to represent the nearest intersection,
      // so if it's not the case, invert t1 and t2
      if (t1 > t2) {
        float w = t1;
        t1 = t2;
        t2 = w;  // swap t1 and t2
      }

      // tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
      if (t2 < tMax) tMax = t2;
      // tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
      if (t1 > tMin) tMin = t1;

      // And here's the trick :
      // If "far" is closer than "near", then there is NO intersection.
      // See the images in the tutorials for the visual explanation.
      if (tMax < tMin) return false;
    } else {
      if ((-e + aabbMinForAxis > 0.0f) || (-e + aabbMaxForAxis < 0.0f)) {
        return false;
      }
    }

    return true;
  };

  if (!testIntersectionOnAxis(Axis::X) || !testIntersectionOnAxis(Axis::Y) ||
      !testIntersectionOnAxis(Axis::Z)) {
    return false;
  } else {
    return true;
  }
}

void OBBIntersection::screenPosToWorldRay(const glm::mat4& view,
                                          const glm::mat4& projection, glm::vec3& origin,
                                          glm::vec3& direction) {
  // first two values of these vectors would need to be normalized between [-1,1]
  // in relation to the screen width and height - hard-coded to 0 because I only
  // want the ray casted from the center of the screen
  glm::vec4 lRayStart_NDC(0.0f, 0.0f, 1.0f, 1.0f);
  glm::vec4 lRayEnd_NDC(0.0f, 0.0f, 0.0f, 1.0f);

  // The Projection matrix goes from Camera Space to NDC.
  // So inverse(ProjectionMatrix) goes from NDC to Camera Space.
  glm::mat4 InverseProjectionMatrix = glm::inverse(projection);

  // The View Matrix goes from World Space to Camera Space.
  // So inverse(ViewMatrix) goes from Camera Space to World Space.
  glm::mat4 InverseViewMatrix = glm::inverse(view);

  glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;
  lRayStart_camera /= lRayStart_camera.w;
  glm::vec4 lRayStart_world = InverseViewMatrix * lRayStart_camera;
  lRayStart_world /= lRayStart_world.w;
  glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;
  lRayEnd_camera /= lRayEnd_camera.w;
  glm::vec4 lRayEnd_world = InverseViewMatrix * lRayEnd_camera;
  lRayEnd_world /= lRayEnd_world.w;

  // Faster way (just one inverse)
  // glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
  // glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
  // glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;

  glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
  lRayDir_world = glm::normalize(lRayDir_world);

  origin = glm::vec3(lRayStart_world);
  direction = glm::normalize(lRayDir_world);
}