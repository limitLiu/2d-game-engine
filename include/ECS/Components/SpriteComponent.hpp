#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include "Common.hpp"

#include <SDL.h>

namespace Engine::ECS {
struct SpriteComponent {
  SDL_Rect srcRect;
  i32 width;
  i32 height;
  std::string assetID;
  i32 zIndex;

  explicit SpriteComponent(const std::string &textureID = "", const i32 width = 0, const i32 height = 0,
                           const i32 zIndex = 0, const i32 srcRectX = 0, const i32 srcRectY = 0)
      : srcRect{srcRectX, srcRectY, width, height}, width(width), height(height), assetID(textureID), zIndex(zIndex) {}
};
} // namespace Engine::ECS

#endif
