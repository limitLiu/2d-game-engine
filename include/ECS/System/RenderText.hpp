#ifndef RENDER_TEXT_SYSTEM_HPP
#define RENDER_TEXT_SYSTEM_HPP

#include "ECS/System/Base.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

namespace Engine::ECS::System {
struct RenderText : public Base {
  RenderText() { requireComponent<Component::TextLabel>(); }

  void update(SDL_Renderer *renderer, const std::unique_ptr<Engine::AssetStore> &store, SDL_Rect &camera) const {
    for (const auto entity : entities()) {
      const auto textLabel = entity.component<Component::TextLabel>();
      auto *font = store->font(textLabel.assetID);
      SDL_Surface *surface = TTF_RenderText_Blended(font, textLabel.text.c_str(), textLabel.color);
      auto *const texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);

      i32 width = 0;
      i32 height = 0;
      SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

      const SDL_Rect dstRect = {
          .x = static_cast<i32>(textLabel.position.x - (textLabel.isFixed ? 0 : camera.x)),
          .y = static_cast<i32>(textLabel.position.y - (textLabel.isFixed ? 0 : camera.y)),
          .w = width,
          .h = height,
      };
      SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    }
  }
};
} // namespace Engine::ECS::System

#endif
