#ifndef PROJECTILE_COMPONENT_HPP
#define PROJECTILE_COMPONENT_HPP

#include "Common.hpp"

#include <SDL.h>
#include <glm/glm.hpp>

namespace Engine::ECS::Component {
struct Projectile {
  bool isFriendly;
  i32 hitPercentDamage;
  i32 duration;
  i32 startTime;

  explicit Projectile(const bool isFriendly = false, const i32 hitPercentDamage = 0, const i32 duration = 0)
      : isFriendly(isFriendly), hitPercentDamage(hitPercentDamage), duration(duration), startTime(SDL_GetTicks()) {}
};
} // namespace Engine::ECS::Component

#endif
