#ifndef DAMAGE_SYSTEM_HPP
#define DAMAGE_SYSTEM_HPP

#include "ECS/Component/BoxCollider.hpp"
#include "ECS/System/Base.hpp"
#include "Event/Bus.hpp"
#include "Event/Collision.hpp"

namespace Engine::ECS::System {
struct Damage : public Base {
  Damage() { requireComponent<Component::BoxCollider>(); }

  void subscribe(const std::unique_ptr<Event::Bus> &eventBus) {
    eventBus->subscribe<Event::Collision>(this, &Damage::onCollision);
  }

  void onCollision(const Event::Collision &event) {
    spdlog::info("Entity {} collied with entity {}", event.a.id(), event.b.id());
    event.a.kill();
    event.b.kill();
  }
};
} // namespace Engine::ECS::System

#endif
