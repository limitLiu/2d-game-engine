#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "ECS/Component/RigidBody.hpp"
#include "ECS/Component/Transform.hpp"
#include "ECS/System/Base.hpp"
#include "Game.hpp"

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

      bool isEntityOutsideMap = transform.position.x < 0 || transform.position.x > Game::s_mapWidth ||
                                transform.position.y < 0 || transform.position.y > Game::s_mapHeight;
      if (isEntityOutsideMap && !entity.hasTag("player")) {
        entity.kill();
      }
    }
  }
};
} // namespace Engine::ECS::System

#endif
