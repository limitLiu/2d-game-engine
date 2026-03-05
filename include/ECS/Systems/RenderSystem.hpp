#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "AssetStore/AssetStore.hpp"
#include "ECS/Base/System.hpp"
#include <SDL.h>

namespace ECS {
struct RenderSystem : public System {
  RenderSystem() {
    requireComponent<TransformComponent>();
    requireComponent<SpriteComponent>();
  }

  void update(SDL_Renderer *context, std::unique_ptr<Engine::AssetStore> &store) {
    for (const auto entity : entities()) {
      const auto &transform = entity.component<TransformComponent>();
      const auto &sprite = entity.component<SpriteComponent>();

      auto *texture = store->textures(sprite.assetID());
      const auto srcRect = sprite.srcRect();
      const SDL_Rect dstRect = {static_cast<i32>(transform.position().x), static_cast<i32>(transform.position().y),
                                static_cast<i32>(sprite.width() * transform.scale().x),
                                static_cast<i32>(sprite.height() * transform.scale().y)};
      SDL_RenderCopyEx(context, texture, &srcRect, &dstRect, transform.rotation(), nullptr, SDL_FLIP_NONE);
    }
  }
};
} // namespace ECS

#endif
