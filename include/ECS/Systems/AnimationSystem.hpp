#ifndef ANIMATION_SYSTEM_HPP
#define ANIMATION_SYSTEM_HPP

#include "ECS/Base/System.hpp"
#include "ECS/Components/AnimationComponent.hpp"
#include "ECS/Components/SpriteComponent.hpp"

namespace Engine::ECS {
struct AnimationSystem : public System {
  AnimationSystem() {
    requireComponent<SpriteComponent>();
    requireComponent<AnimationComponent>();
  }

  void update(const f64 deltaTime) {
    for (const auto entity : entities()) {
      auto &sprite = entity.component<SpriteComponent>();
      auto &animation = entity.component<AnimationComponent>();
      animation.currentFrame =
          (SDL_GetTicks() - animation.startedAt) * animation.frameSpeedRate / 1000 % animation.frames;
      sprite.srcRect.x = animation.currentFrame * sprite.width;
    }
  }
};
} // namespace Engine::ECS

#endif
