#ifndef PROJECTILE_EMITTER_SYSTEM_HPP
#define PROJECTILE_EMITTER_SYSTEM_HPP

#include "ECS/Base/Registry.hpp"
#include "ECS/Component/BoxCollider.hpp"
#include "ECS/Component/CameraFollow.hpp"
#include "ECS/Component/Projectile.hpp"
#include "ECS/Component/ProjectileEmitter.hpp"
#include "ECS/Component/RigidBody.hpp"
#include "ECS/Component/Sprite.hpp"
#include "ECS/Component/Transform.hpp"
#include "ECS/System/Base.hpp"
#include "Event/Bus.hpp"
#include "Event/Keypressed.hpp"

#include <SDL.h>
#include <SDL_keycode.h>
#include <memory>

namespace Engine::ECS::System {
struct ProjectileEmitter : public Base {
  ProjectileEmitter() {
    requireComponent<Component::Transform>();
    requireComponent<Component::ProjectileEmitter>();
  }

  void subscribe(const std::unique_ptr<Event::Bus> &eventBus) {
    eventBus->subscribe<Event::KeyPressed>(this, &ProjectileEmitter::onKeyPressed);
  }

  void onKeyPressed(const Event::KeyPressed &event) const {
    if (event.symbol == SDLK_SPACE) {
      for (auto entity : entities()) {
        if (entity.hasComponent<Component::CameraFollow>()) {
          auto &projectileEmitter = entity.component<Component::ProjectileEmitter>();
          const auto &transform = entity.component<Component::Transform>();
          const auto &rigidBody = entity.component<Component::RigidBody>();
          auto projectilePosition = transform.position;
          if (entity.hasComponent<ECS::Component::Sprite>()) {
            const auto &sprite = entity.component<ECS::Component::Sprite>();
            projectilePosition.x += transform.scale.x * sprite.width * 0.5;
            projectilePosition.y += transform.scale.y * sprite.height * 0.5;
          }
          auto velocity = projectileEmitter.velocity;
          i32 directionX = 0;
          i32 directionY = 0;
          if (rigidBody.velocity.x > 0) {
            directionX = 1;
          }
          if (rigidBody.velocity.x < 0) {
            directionX = -1;
          }
          if (rigidBody.velocity.y > 0) {
            directionY = 1;
          }
          if (rigidBody.velocity.y < 0) {
            directionY = -1;
          }
          velocity.x = projectileEmitter.velocity.x * directionX;
          velocity.y = projectileEmitter.velocity.y * directionY;

          Entity projectile = entity.registry->createEntity();
          projectile.setGroup("projectiles");
          projectile.addComponent<ECS::Component::Transform>(projectilePosition, glm::vec2(1.0, 1.0));
          projectile.addComponent<ECS::Component::RigidBody>(velocity);
          projectile.addComponent<ECS::Component::Sprite>("bullet", 4, 4, 4);
          projectile.addComponent<ECS::Component::BoxCollider>(4, 4);
          projectile.addComponent<ECS::Component::Projectile>(
              projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.duration);
        }
      }
    }
  }

  void update(const std::unique_ptr<Registry> &registry) const {
    for (const auto entity : entities()) {
      const auto &transform = entity.component<Component::Transform>();
      auto &projectileEmitter = entity.component<Component::ProjectileEmitter>();
      if (projectileEmitter.repeatFrequency == 0) {
        continue;
      }
      if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency) {
        Entity projectile = registry->createEntity();
        projectile.setGroup("projectiles");
        auto projectilePosition = transform.position;
        if (entity.hasComponent<ECS::Component::Sprite>()) {
          const auto &sprite = entity.component<ECS::Component::Sprite>();
          projectilePosition.x += transform.scale.x * sprite.width * 0.5;
          projectilePosition.y += transform.scale.y * sprite.height * 0.5;
        }
        projectile.addComponent<ECS::Component::Transform>(projectilePosition, glm::vec2(1.0, 1.0));
        projectile.addComponent<ECS::Component::RigidBody>(projectileEmitter.velocity);
        projectile.addComponent<ECS::Component::Sprite>("bullet", 4, 4, 4);
        projectile.addComponent<ECS::Component::BoxCollider>(4, 4);
        projectile.addComponent<ECS::Component::Projectile>(
            projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.duration);
        projectileEmitter.lastEmissionTime = SDL_GetTicks();
      }
    }
  }
};
} // namespace Engine::ECS::System

#endif
