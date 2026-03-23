#ifndef PROJECTILE_EMITTER_SYSTEM_HPP
#define PROJECTILE_EMITTER_SYSTEM_HPP

#include "ECS/Component/BoxCollider.hpp"
#include "ECS/Component/ProjectileEmitter.hpp"
#include "ECS/Component/RigidBody.hpp"
#include "ECS/Component/Sprite.hpp"
#include "ECS/Component/Transform.hpp"
#include "ECS/System/Base.hpp"

#include <SDL.h>

namespace Engine::ECS::System {
struct ProjectileEmitter : public Base {
  ProjectileEmitter() {
    requireComponent<Component::Transform>();
    requireComponent<Component::ProjectileEmitter>();
  }

  void update(const std::unique_ptr<Registry> &registry) const {
    for (const auto entity : entities()) {
      auto &transform = entity.component<Component::Transform>();
      if (auto &projectileEmitter = entity.component<Component::ProjectileEmitter>();
          SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency) {
        Entity projectile = registry->createEntity();
        auto projectilePosition = transform.position;
        if (entity.hasComponent<ECS::Component::Sprite>()) {
          auto &sprite = entity.component<ECS::Component::Sprite>();
          projectilePosition.x += transform.scale.x * sprite.width * 0.5;
          projectilePosition.y += transform.scale.y * sprite.height * 0.5;
        }
        projectile.addComponent<ECS::Component::Transform>(projectilePosition, glm::vec2(1.0, 1.0));
        projectile.addComponent<ECS::Component::RigidBody>(projectileEmitter.velocity);
        projectile.addComponent<ECS::Component::Sprite>("bullet", 4, 4, 4);
        projectile.addComponent<ECS::Component::BoxCollider>(4, 4);
        projectileEmitter.lastEmissionTime = SDL_GetTicks();
      }
    }
  }
};
} // namespace Engine::ECS::System

#endif
