#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "ECS/Component/RigidBody.hpp"
#include "ECS/Component/Transform.hpp"
#include "ECS/System/Base.hpp"

namespace Engine::ECS::System {
struct Movement : public Base {
  Movement() {
    requireComponent<Component::Transform>();
    requireComponent<Component::RigidBody>();
  }

  void update(const f64 deltaTime) const {
    for (const auto entity : entities()) {
      auto &transform = entity.component<Component::Transform>();
      const auto &rigidBody = entity.component<Component::RigidBody>();
      transform.position.x += rigidBody.velocity.x * deltaTime;
      transform.position.y += rigidBody.velocity.y * deltaTime;
    }
  }
};
} // namespace Engine::ECS::System

#endif
