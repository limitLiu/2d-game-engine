#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "ECS/Component/BoxCollider.hpp"
#include "ECS/Component/Transform.hpp"
#include "ECS/System/Base.hpp"
#include "Event/Bus.hpp"
#include "Event/Collision.hpp"
#include <spdlog/spdlog.h>

namespace Engine::ECS::System {
struct Collision : public Base {
  Collision() {
    requireComponent<Component::BoxCollider>();
    requireComponent<Component::Transform>();
  }

  void update(const std::unique_ptr<Event::Bus> &eventBus) const {
    const auto elements = entities();
    for (auto i = elements.begin(); i != elements.end(); ++i) {
      Entity a = *i;
      const auto aTransform = a.component<Component::Transform>();
      const auto aCollider = a.component<Component::BoxCollider>();
      for (auto j = i; j != elements.end(); ++j) {
        Entity b = *j;
        if (a == b) {
          continue;
        }
        const auto bTransform = b.component<Component::Transform>();
        const auto bCollider = b.component<Component::BoxCollider>();
        const auto collisionHappened =
            CheckAABBCollision(aTransform.position.x, aTransform.position.y, aCollider.width, aCollider.height,
                               bTransform.position.x, bTransform.position.y, bCollider.width, bCollider.height);
        if (collisionHappened) {
          spdlog::info("Entity {} is colliding with entity {}", a.id(), b.id());
          eventBus->emit<Event::Collision>(a, b);
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
} // namespace Engine::ECS::System

#endif
