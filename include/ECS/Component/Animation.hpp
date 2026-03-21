#ifndef ANIMATION_COMPONENT_HPP
#define ANIMATION_COMPONENT_HPP

#include "Common.hpp"
#include <SDL.h>

namespace Engine::ECS::Component {
struct Animation {
  i32 currentFrame;
  i32 frames;
  i32 frameSpeedRate;
  bool isLoop;
  i32 startedAt;

  explicit Animation(const i32 frames = 1, const i32 frameSpeedRate = 1, const bool isLoop = true)
      : currentFrame(1), frames(frames), frameSpeedRate(frameSpeedRate), isLoop(isLoop), startedAt(SDL_GetTicks()) {}
};
} // namespace Engine::ECS::Component

#endif
