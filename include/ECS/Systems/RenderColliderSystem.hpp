#ifndef RENDER_COLLIDER_SYSTEM_HPP
#define RENDER_COLLIDER_SYSTEM_HPP

#include "ECS/Base/System.hpp"
#include "ECS/Components/BoxColliderComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "spdlog/spdlog.h"

#include <SDL.h>

namespace Engine::ECS {
struct RenderColliderSystem : public System {
  RenderColliderSystem() {
    requireComponent<BoxColliderComponent>();
    requireComponent<TransformComponent>();
  }

  void update(SDL_Renderer *renderer) {
    for (const auto entity : entities()) {
      const auto transform = entity.component<TransformComponent>();
      const auto collider = entity.component<BoxColliderComponent>();
      SDL_Rect rect = {
          static_cast<i32>(transform.position.x + collider.offset.x),
          static_cast<i32>(transform.position.y + collider.offset.y),
          static_cast<i32>(collider.width),
          static_cast<i32>(collider.height),
      };
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_RenderDrawRect(renderer, &rect);
    }
  }
};
} // namespace Engine::ECS

#endif
