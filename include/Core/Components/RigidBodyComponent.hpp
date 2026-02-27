#ifndef RIGID_BODY_COMPONENT_HPP
#define RIGID_BODY_COMPONENT_HPP

#include "Common.hpp"
#include <glm/glm.hpp>

namespace ecs {
struct RigidBodyComponent {
  glm::vec2 velocity;
  RigidBodyComponent(const glm::vec2 velocity = glm::vec2(0, 0)) : velocity(velocity) {}
};
}; // namespace ecs

#endif
