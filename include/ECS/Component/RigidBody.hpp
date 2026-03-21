#ifndef RIGID_BODY_COMPONENT_HPP
#define RIGID_BODY_COMPONENT_HPP

#include <glm/glm.hpp>

namespace Engine::ECS::Component {
struct RigidBody {
  glm::vec2 velocity;

  explicit RigidBody(const glm::vec2 velocity = glm::vec2(0, 0)) : velocity(velocity) {}
};
} // namespace Engine::ECS::Component

#endif
