#ifndef DAMAGE_SYSTEM_HPP
#define DAMAGE_SYSTEM_HPP

#include "ECS/Component/BoxCollider.hpp"
#include "ECS/Component/Health.hpp"
#include "ECS/Component/Projectile.hpp"
#include "ECS/System/Base.hpp"
#include "Event/Bus.hpp"
#include "Event/Collision.hpp"

#include <spdlog/spdlog.h>

namespace Engine::ECS::System {
struct Damage : public Base {
  Damage() { requireComponent<Component::BoxCollider>(); }

  void subscribe(const std::unique_ptr<Event::Bus> &eventBus) {
    eventBus->subscribe<Event::Collision>(this, &Damage::onCollision);
  }

  void onCollision(const Event::Collision &event) const {
    const auto a = event.a;
    const auto b = event.b;
    spdlog::info("Entity {} collied with entity {}", a.id(), b.id());
    if (a.belongsToGroup("projectiles") && b.hasTag("player")) {
      projectileHitsPlayer(a, b);
    }
    if (b.belongsToGroup("projectiles") && a.hasTag("player")) {
      projectileHitsPlayer(b, a);
    }

    if (a.belongsToGroup("projectiles") && b.belongsToGroup("enemies")) {
      projectileHitsEnemy(a, b);
    }
    if (b.belongsToGroup("projectiles") && a.belongsToGroup("enemies")) {
      projectileHitsEnemy(b, a);
    }
  }

private:
  static void projectileHitsPlayer(const Entity projectile, const Entity player) {
    const auto projectileComponent = projectile.component<Component::Projectile>();
    if (!projectileComponent.isFriendly) {
      auto &healthComponent = player.component<Component::Health>();
      healthComponent.percentage -= projectileComponent.hitPercentDamage;
      if (healthComponent.percentage <= 0) {
        player.kill();
      }
      projectile.kill();
    }
  }

  static void projectileHitsEnemy(const Entity projectile, const Entity enemy) {
    const auto projectileComponent = projectile.component<Component::Projectile>();
    if (projectileComponent.isFriendly) {
      auto &healthComponent = enemy.component<Component::Health>();
      healthComponent.percentage -= projectileComponent.hitPercentDamage;
      spdlog::info("{}", healthComponent.percentage);
      if (healthComponent.percentage <= 0) {
        enemy.kill();
      }
      projectile.kill();
    }
  }
};
} // namespace Engine::ECS::System

#endif
