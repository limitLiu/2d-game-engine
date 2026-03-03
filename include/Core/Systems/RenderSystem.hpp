#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "Core/Base/System.hpp"
#include <SDL.h>
#include <SDL_image.h>

namespace ecs {
struct RenderSystem : public System {
  RenderSystem() {
    requireComponent<TransformComponent>();
    requireComponent<SpriteComponent>();
  }

  void update(SDL_Renderer *context) {
    for (const auto entity : entities()) {
      const auto &transform = entity.component<TransformComponent>();
      const auto &sprite = entity.component<SpriteComponent>();
      const SDL_Rect dstRect = {static_cast<i32>(transform.position().x), static_cast<i32>(transform.position().y),
                                sprite.width(), sprite.height()};
      SDL_SetRenderDrawColor(context, 255, 255, 255, SDL_ALPHA_OPAQUE);
      SDL_RenderFillRect(context, &dstRect);
    }
  }
};
} // namespace ecs

#endif
