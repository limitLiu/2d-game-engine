#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "Common.hpp"
#include <glm/glm.hpp>

namespace ecs {
struct TransformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  f64 rotation;
};
} // namespace ecs

#endif
