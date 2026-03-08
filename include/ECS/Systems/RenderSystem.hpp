#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "AssetStore/AssetStore.hpp"
#include "ECS/Base/System.hpp"
#include "ECS/Components/SpriteComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"

#include <SDL.h>
#include <algorithm>
#include <memory>
#include <vector>

namespace ECS {
struct RenderSystem : public System {
  RenderSystem() {
    requireComponent<struct TransformComponent>();
    requireComponent<struct SpriteComponent>();
  }

  void update(SDL_Renderer *context, std::unique_ptr<Engine::AssetStore> &store) {
    struct RenderableEntity {
      TransformComponent transformComponent;
      SpriteComponent spriteComponent;
    };
    std::vector<RenderableEntity> renderableEntities;
    for (const auto entity : entities()) {
      RenderableEntity e;
      e.transformComponent = entity.component<TransformComponent>();
      e.spriteComponent = entity.component<SpriteComponent>();
      renderableEntities.emplace_back(e);
    }

    std::sort(renderableEntities.begin(), renderableEntities.end(),
              [](const RenderableEntity &a, const RenderableEntity &b) {
                return a.spriteComponent.zIndex() < b.spriteComponent.zIndex();
              });

    for (const auto entity : renderableEntities) {
      const auto transformComponent = entity.transformComponent;
      const auto spriteComponent = entity.spriteComponent;
      auto *texture = store->textures(spriteComponent.assetID());
      const auto srcRect = spriteComponent.srcRect();
      const SDL_Rect dstRect = {static_cast<i32>(transformComponent.position().x),
                                static_cast<i32>(transformComponent.position().y),
                                static_cast<i32>(spriteComponent.width() * transformComponent.scale().x),
                                static_cast<i32>(spriteComponent.height() * transformComponent.scale().y)};
      SDL_RenderCopyEx(context, texture, &srcRect, &dstRect, transformComponent.rotation(), nullptr, SDL_FLIP_NONE);
    }
  }
};
} // namespace ECS

#endif
