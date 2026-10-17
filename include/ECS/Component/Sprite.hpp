#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include "Common.hpp"

#include <SDL.h>

namespace Engine::ECS::Component {
struct Sprite {
  std::string assetID;
  i32 width;
  i32 height;
  i32 zIndex;
  bool isFixed;
  SDL_Rect srcRect;

  explicit Sprite(const std::string &textureID = "", const i32 width = 0, const i32 height = 0, const i32 zIndex = 0,
                  const bool isFixed = false, const i32 srcRectX = 0, const i32 srcRectY = 0)
      : assetID(textureID), width(width), height(height), zIndex(zIndex), isFixed(isFixed),
        srcRect{.x = srcRectX, .y = srcRectY, .w = width, .h = height} {}
};
} // namespace Engine::ECS::Component

#endif
