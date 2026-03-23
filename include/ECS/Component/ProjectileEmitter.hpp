#ifndef PROJECTILE_EMITTER_COMPONENT_HPP
#define PROJECTILE_EMITTER_COMPONENT_HPP

#include "Common.hpp"

#include <SDL.h>
#include <glm/glm.hpp>

namespace Engine::ECS::Component {
struct ProjectileEmitter {
  glm::vec2 velocity;
  i32 repeatFrequency;
  i32 duration;
  i32 hitPercentDamage;
  bool isFriendly;
  i32 lastEmissionTime;

  explicit ProjectileEmitter(const glm::vec2 velocity = glm::vec2(0), const i32 repeatFrequency = 0,
                             const i32 duration = 10000, const i32 hitPercentDamage = 10, const bool isFriendly = false)
      : velocity(velocity), repeatFrequency(repeatFrequency), duration(duration), hitPercentDamage(hitPercentDamage),
        isFriendly(isFriendly), lastEmissionTime(SDL_GetTicks()) {}
};
} // namespace Engine::ECS::Component

#endif
