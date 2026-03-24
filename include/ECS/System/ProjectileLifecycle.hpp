#ifndef PROJECTILE_LIFECYCLE_SYSTEM_HPP
#define PROJECTILE_LIFECYCLE_SYSTEM_HPP

#include "ECS/Component/Projectile.hpp"
#include "ECS/System/Base.hpp"

#include <SDL.h>

namespace Engine::ECS::System {
struct ProjectileLifecycle : public Base {
  ProjectileLifecycle() { requireComponent<Component::Projectile>(); }

  void update() const {
    for (auto entity : entities()) {
      if (const auto &projectile = entity.component<Component::Projectile>();
          SDL_GetTicks() - projectile.startTime > projectile.duration) {
        entity.kill();
      }
    }
  }
};
} // namespace Engine::ECS::System

#endif
