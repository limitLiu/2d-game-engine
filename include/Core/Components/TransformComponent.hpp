#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "Common.hpp"
#include <glm/glm.hpp>

namespace ecs {
struct TransformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  f64 rotation;

  TransformComponent(glm::vec2 position = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1, 1), f64 rotation = 0.0) {
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
  }
};
} // namespace ecs

#endif
