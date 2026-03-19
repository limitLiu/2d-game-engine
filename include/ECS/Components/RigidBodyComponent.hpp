#ifndef RIGID_BODY_COMPONENT_HPP
#define RIGID_BODY_COMPONENT_HPP

#include <glm/glm.hpp>

namespace Engine::ECS {
struct RigidBodyComponent {
  glm::vec2 velocity;

  explicit RigidBodyComponent(const glm::vec2 velocity = glm::vec2(0, 0)) : velocity(velocity) {}
};
} // namespace Engine::ECS

#endif
