#ifndef BoxCollider_COMPONENT_HPP
#define BoxCollider_COMPONENT_HPP

#include "Common.hpp"
#include <glm/glm.hpp>

namespace Engine::ECS::Component {
struct BoxCollider {
  i32 width;
  i32 height;
  glm::vec2 offset;

  explicit BoxCollider(const i32 width = 0, const i32 height = 0, const glm::vec2 offset = glm::vec2(0, 0))
      : width(width), height(height), offset(offset) {}
};
} // namespace Engine::ECS::Component

#endif
