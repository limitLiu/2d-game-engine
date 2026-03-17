#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "ECS/Base/System.hpp"
#include "ECS/Components/BoxColliderComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "spdlog/spdlog.h"

namespace ECS {
struct CollisionSystem : public System {
  CollisionSystem() {
    requireComponent<BoxColliderComponent>();
    requireComponent<TransformComponent>();
  }

  void update() {
    const auto elements = entities();
    for (auto i = elements.begin(); i != elements.end(); ++i) {
      Entity a = *i;
      const auto aTransform = a.component<TransformComponent>();
      const auto aCollider = a.component<BoxColliderComponent>();
      for (auto j = i; j != elements.end(); ++j) {
        Entity b = *j;
        if (a == b) {
          continue;
        }
        const auto bTransform = b.component<TransformComponent>();
        const auto bCollider = b.component<BoxColliderComponent>();
        const auto collisionHappened =
            CheckAABBCollision(aTransform.position().x, aTransform.position().y, aCollider.width(), aCollider.height(),
                               bTransform.position().x, bTransform.position().y, bCollider.width(), bCollider.height());
        if (collisionHappened) {
          spdlog::info("Entity {} is colliding with entity {}", a.id(), b.id());
          a.kill();
          b.kill();
        }
      }
    }
  }

private:
  static bool CheckAABBCollision(const f64 aX, const f64 aY, const f64 aWidth, const f64 aHeight, const f64 bX,
                                 const f64 bY, const f64 bWidth, const f64 bHeight) {
    return aX < bX + bWidth && aX + aWidth > bX && aY < bY + bHeight && aY + aHeight > bY;
  }
};
} // namespace ECS

#endif
