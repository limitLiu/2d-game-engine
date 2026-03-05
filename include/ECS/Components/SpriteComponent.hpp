#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include "Common.hpp"

#include <SDL.h>

namespace ECS {
struct SpriteComponent {
  i32 width() const { return m_width; }

  i32 height() const { return m_height; }

  std::string assetID() const { return m_assetID; }

  SDL_Rect srcRect() const { return m_srcRect; }

  explicit SpriteComponent(const std::string &textureID = "", const i32 width = 0, const i32 height = 0,
                           const i32 srcRectX = 0, const i32 srcRectY = 0)
      : m_width(width), m_height(height), m_assetID(textureID), m_srcRect(srcRectX, srcRectY, width, height) {}

private:
  i32 m_width;

  i32 m_height;

  std::string m_assetID;

  SDL_Rect m_srcRect;
};
} // namespace ECS

#endif
