#ifndef KEYBOARD_CONTROLLED_COMPONENT_HPP
#define KEYBOARD_CONTROLLED_COMPONENT_HPP

#include <glm/glm.hpp>

namespace Engine::ECS::Component {
struct KeyboardControlled {
  glm::vec2 upVelocity;
  glm::vec2 rightVelocity;
  glm::vec2 downVelocity;
  glm::vec2 leftVelocity;

  explicit KeyboardControlled(const glm::vec2 upVelocity = glm::vec2(0), const glm::vec2 rightVelocity = glm::vec2(0),
                              const glm::vec2 downVelocity = glm::vec2(0), const glm::vec2 leftVelocity = glm::vec2(0))
      : upVelocity(upVelocity), rightVelocity(rightVelocity), downVelocity(downVelocity), leftVelocity(leftVelocity) {}
};
} // namespace Engine::ECS::Component

#endif
