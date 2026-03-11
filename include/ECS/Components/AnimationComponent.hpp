#ifndef ANIMATION_COMPONENT_HPP
#define ANIMATION_COMPONENT_HPP

#include "Common.hpp"
#include <SDL.h>

namespace ECS {
struct AnimationComponent {
  AnimationComponent(const i32 frames = 1, const i32 frameSpeedRate = 1, const bool isLoop = true)
      : m_currentFrame(1), m_frames(frames), m_frameSpeedRate(frameSpeedRate), m_isLoop(isLoop),
        m_startedAt(SDL_GetTicks()) {}

  i32 frames() const { return m_frames; }
  i32 frameSpeedRate() const { return m_frameSpeedRate; }
  i32 currentFrame() const { return m_currentFrame; }
  bool isLoop() const { return m_isLoop; }
  i32 startedAt() const { return m_startedAt; }

  i32 m_currentFrame;

private:
  i32 m_frames;
  i32 m_frameSpeedRate;
  bool m_isLoop;
  i32 m_startedAt;
};
} // namespace ECS

#endif
