#ifndef COLLISION_EVENT_HPP
#define COLLISION_EVENT_HPP

#include "Common.hpp"
#include "ECS/Base/Entity.hpp"

namespace Engine::Event {
struct Collision {
  ECS::Entity a;
  ECS::Entity b;
  Collision(const ECS::Entity a, const ECS::Entity b) : a(a), b(b) {};
};
} // namespace Engine::Event

#endif
