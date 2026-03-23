#ifndef RENDER_COLLIDER_SYSTEM_HPP
#define RENDER_COLLIDER_SYSTEM_HPP

#include "ECS/Component/BoxCollider.hpp"
#include "ECS/Component/Transform.hpp"
#include "ECS/System/Base.hpp"

#include <SDL.h>

namespace Engine::ECS::System {
struct RenderCollider : public Base {
  RenderCollider() {
    requireComponent<Component::BoxCollider>();
    requireComponent<Component::Transform>();
  }

  void update(SDL_Renderer *renderer, const SDL_Rect &camera) const {
    for (const auto entity : entities()) {
      const auto transform = entity.component<Component::Transform>();
      const auto collider = entity.component<Component::BoxCollider>();
      SDL_Rect rect = {
          static_cast<i32>(transform.position.x + collider.offset.x - camera.x),
          static_cast<i32>(transform.position.y + collider.offset.y - camera.y),
          static_cast<i32>(collider.width * transform.scale.x),
          static_cast<i32>(collider.height * transform.scale.y),
      };
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_RenderDrawRect(renderer, &rect);
    }
  }
};
} // namespace Engine::ECS::System

#endif
