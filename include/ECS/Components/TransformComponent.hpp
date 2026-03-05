#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "Common.hpp"
#include <glm/glm.hpp>

namespace ECS {
struct TransformComponent {
  glm::vec2 position() const { return m_position; }
  glm::vec2 scale() const { return m_scale; }
  f64 rotation() const { return m_rotation; }

  explicit TransformComponent(const glm::vec2 position = glm::vec2(0, 0), const glm::vec2 scale = glm::vec2(1, 1),
                              const f64 rotation = 0.0)
      : m_position(position), m_scale(scale), m_rotation(rotation) {}

  glm::vec2 m_position;
  glm::vec2 m_scale;
  f64 m_rotation;
};
} // namespace ECS

#endif
