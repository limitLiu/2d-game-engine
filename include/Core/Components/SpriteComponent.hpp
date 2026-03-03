#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include "Common.hpp"

namespace ecs {
struct SpriteComponent {
  i32 width() const { return m_width; }
  i32 height() const { return m_height; }

  explicit SpriteComponent(const i32 width = 0, const i32 height = 0) : m_width(width), m_height(height) {}

private:
  i32 m_width;
  i32 m_height;
};
} // namespace ecs

#endif
