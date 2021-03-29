#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OBBIntersection {
 public:
  static bool isIntersection(const glm::vec3& origin,
                             const glm::vec3& normalizedDirection,
                             const glm::vec3& aabbMin, const glm::vec3& aabbMax,
                             const glm::mat4& model, float& intersectionDistance);

  static void screenPosToWorldRay(const glm::mat4& view, const glm::mat4& projection,
                                  glm::vec3& origin, glm::vec3& direction);

 private:
  OBBIntersection() {}
  enum class Axis { X, Y, Z };
};