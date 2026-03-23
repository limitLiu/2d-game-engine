#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP

#include "Common.hpp"

namespace Engine::ECS::Component {
struct Health {
  i32 percentage;
  explicit Health(const i32 percentage = 0) : percentage(percentage) {}
};
} // namespace Engine::ECS::Component

#endif
