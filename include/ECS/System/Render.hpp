#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "AssetStore/AssetStore.hpp"
#include "ECS/Component/Sprite.hpp"
#include "ECS/Component/Transform.hpp"
#include "ECS/System/Base.hpp"

#include <SDL.h>
#include <algorithm>
#include <memory>
#include <vector>

namespace Engine::ECS::System {
struct Render : public Base {
  Render() {
    requireComponent<Component::Transform>();
    requireComponent<Component::Sprite>();
  }

  void update(SDL_Renderer *context, std::unique_ptr<Engine::AssetStore> &store, SDL_Rect &camera) const {
    struct RenderableEntity {
      Component::Transform transformComponent;
      Component::Sprite spriteComponent;
    };
    std::vector<RenderableEntity> renderableEntities;
    for (const auto entity : entities()) {
      RenderableEntity e;
      e.transformComponent = entity.component<Component::Transform>();
      e.spriteComponent = entity.component<Component::Sprite>();
      renderableEntities.emplace_back(e);
    }

    std::sort(renderableEntities.begin(), renderableEntities.end(),
              [](const RenderableEntity &a, const RenderableEntity &b) {
                return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
              });

    for (const auto [transformComponent, spriteComponent] : renderableEntities) {
      auto *texture = store->textures(spriteComponent.assetID);
      const auto srcRect = spriteComponent.srcRect;
      const SDL_Rect dstRect = {
          static_cast<i32>(transformComponent.position.x - (spriteComponent.isFixed ? 0 : camera.x)),
          static_cast<i32>(transformComponent.position.y - (spriteComponent.isFixed ? 0 : camera.y)),
          static_cast<i32>(spriteComponent.width * transformComponent.scale.x),
          static_cast<i32>(spriteComponent.height * transformComponent.scale.y)};
      SDL_RenderCopyEx(context, texture, &srcRect, &dstRect, transformComponent.rotation, nullptr, SDL_FLIP_NONE);
    }
  }
};
} // namespace Engine::ECS::System

#endif
