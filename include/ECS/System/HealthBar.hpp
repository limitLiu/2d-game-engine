#ifndef HEALTH_BAR_SYSTEM_HPP
#define HEALTH_BAR_SYSTEM_HPP

#include "AssetStore/AssetStore.hpp"
#include "ECS/Component/Health.hpp"
#include "ECS/Component/Sprite.hpp"
#include "ECS/Component/Transform.hpp"
#include "ECS/System/Base.hpp"

#include <SDL.h>
#include <string>

namespace Engine::ECS::System {
struct HealthBar : public Base {
  HealthBar() {
    requireComponent<Component::Transform>();
    requireComponent<Component::Sprite>();
    requireComponent<Component::Health>();
  }

  void update(SDL_Renderer *renderer, const std::unique_ptr<Engine::AssetStore> &store, const SDL_Rect &camera) const {
    for (const auto entity : entities()) {
      const auto transform = entity.component<Component::Transform>();
      const auto sprite = entity.component<Component::Sprite>();
      const auto health = entity.component<Component::Health>();
      SDL_Color color;
      if (health.percentage >= 0 && health.percentage < 40) {
        color = {.r = 255, .g = 0, .b = 0};
      } else if (health.percentage >= 40 && health.percentage < 80) {
        color = {.r = 255, .g = 255, .b = 0};
      } else if (health.percentage >= 80 && health.percentage <= 100) {
        color = {.r = 0, .g = 255, .b = 0};
      } else {
        color = {.r = 255, .g = 255, .b = 255};
      }
      constexpr i32 healthBarWidth = 15;
      constexpr i32 healthBarHeight = 3;
      const f64 healthBarPositionX = (transform.position.x + sprite.width * transform.scale.x) - camera.x;
      const f64 healthBarPositionY = transform.position.y - camera.y;

      SDL_Rect healthBarRect = {
          .x = static_cast<i32>(healthBarPositionX),
          .y = static_cast<i32>(healthBarPositionY),
          .w = static_cast<i32>(healthBarWidth * health.percentage / 100.0),
          .h = static_cast<i32>(healthBarHeight),
      };
      SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
      SDL_RenderFillRect(renderer, &healthBarRect);

      const auto healthText = std::to_string(health.percentage);
      auto *font = store->font("pico8-5");
      SDL_Surface *surface = TTF_RenderText_Blended(font, healthText.c_str(), color);
      auto *const texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);

      i32 width = 0;
      i32 height = 0;
      SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

      const SDL_Rect dstRect = {
          .x = static_cast<i32>(healthBarPositionX),
          .y = static_cast<i32>(healthBarPositionY + 5),
          .w = width,
          .h = height,
      };
      SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

      SDL_DestroyTexture(texture);
    }
  }
};
} // namespace Engine::ECS::System

#endif
