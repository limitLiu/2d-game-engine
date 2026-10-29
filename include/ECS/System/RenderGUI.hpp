#ifndef RENDER_GUI_SYSTEM_HPP
#define RENDER_GUI_SYSTEM_HPP

#include "ECS/System/Base.hpp"

#include <SDL.h>

namespace Engine::ECS {
struct Registry;
namespace System {
struct RenderGUI : public Base {
  RenderGUI() = default;

  auto update(SDL_Renderer *renderer, const std::unique_ptr<Registry> &registry, const SDL_Rect &camera) const -> void;
};
} // namespace System
} // namespace Engine::ECS

#endif
