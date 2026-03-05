#ifndef RIGID_BODY_COMPONENT_HPP
#define RIGID_BODY_COMPONENT_HPP

#include <glm/glm.hpp>

namespace ECS {
struct RigidBodyComponent {
  glm::vec2 m_velocity;
  glm::vec2 velocity() const { return m_velocity; }

  explicit RigidBodyComponent(const glm::vec2 velocity = glm::vec2(0, 0)) : m_velocity(velocity) {}
};
} // namespace ECS

#endif
