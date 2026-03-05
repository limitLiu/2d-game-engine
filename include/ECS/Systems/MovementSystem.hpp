#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "ECS/Base/System.hpp"
#include "ECS/Components/RigidBodyComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"

namespace ECS {
struct MovementSystem : public System {
  MovementSystem() {
    requireComponent<TransformComponent>();
    requireComponent<RigidBodyComponent>();
  }

  void update(const f64 deltaTime) {
    for (const auto entity : entities()) {
      auto &transform = entity.component<TransformComponent>();
      const auto &rigidBody = entity.component<RigidBodyComponent>();
      transform.m_position.x += rigidBody.velocity().x * deltaTime;
      transform.m_position.y += rigidBody.velocity().y * deltaTime;
    }
  }
};
} // namespace ECS

#endif
