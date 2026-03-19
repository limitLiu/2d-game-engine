#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "Common.hpp"
#include <glm/glm.hpp>

namespace Engine::ECS {
struct TransformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  f64 rotation;

  explicit TransformComponent(const glm::vec2 position = glm::vec2(0, 0), const glm::vec2 scale = glm::vec2(1, 1),
                              const f64 rotation = 0.0)
      : position(position), scale(scale), rotation(rotation) {}
};
} // namespace Engine::ECS

#endif
