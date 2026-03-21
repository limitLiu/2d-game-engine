#ifndef ANIMATION_SYSTEM_HPP
#define ANIMATION_SYSTEM_HPP

#include "ECS/Component/Animation.hpp"
#include "ECS/Component/Sprite.hpp"
#include "ECS/System/Base.hpp"

namespace Engine::ECS::System {
struct Animation : public Base {
  Animation() {
    requireComponent<Component::Sprite>();
    requireComponent<Component::Animation>();
  }

  void update(const f64 deltaTime) const {
    for (const auto entity : entities()) {
      auto &sprite = entity.component<Component::Sprite>();
      auto &animation = entity.component<Component::Animation>();
      animation.currentFrame =
          (SDL_GetTicks() - animation.startedAt) * animation.frameSpeedRate / 1000 % animation.frames;
      sprite.srcRect.x = animation.currentFrame * sprite.width;
    }
  }
};
} // namespace Engine::ECS::System

#endif
