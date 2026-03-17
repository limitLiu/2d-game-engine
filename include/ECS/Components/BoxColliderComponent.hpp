#ifndef BoxCollider_COMPONENT_HPP
#define BoxCollider_COMPONENT_HPP

#include "Common.hpp"
#include <glm/glm.hpp>

namespace ECS {
struct BoxColliderComponent {
  explicit BoxColliderComponent(const i32 width = 0, const i32 height = 0, const glm::vec2 offset = glm::vec2(0, 0))
      : m_width(width), m_height(height), m_offset(offset) {}

  i32 width() const { return m_width; }
  i32 height() const { return m_height; }
  glm::vec2 offset() const { return m_offset; }

private:
  i32 m_width;
  i32 m_height;
  glm::vec2 m_offset;
};
} // namespace ECS

#endif
