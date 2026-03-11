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

  i32 zIndex() const { return m_zIndex; }

  explicit SpriteComponent(const std::string &textureID = "", const i32 width = 0, const i32 height = 0, i32 zIndex = 0,
                           const i32 srcRectX = 0, const i32 srcRectY = 0)
      : m_srcRect(srcRectX, srcRectY, width, height), m_width(width), m_height(height), m_assetID(textureID),
        m_zIndex(zIndex) {}

  SDL_Rect m_srcRect;

private:
  i32 m_width;

  i32 m_height;

  std::string m_assetID;

  i32 m_zIndex;
};
} // namespace ECS

#endif
