#ifndef RENDER_GUI_SYSTEM_HPP
#define RENDER_GUI_SYSTEM_HPP

#include "ECS/System/Base.hpp"

#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

namespace Engine::ECS::System {
struct RenderGUI : public Base {
  RenderGUI() = default;

  void update(SDL_Renderer *renderer) const {
    SDL_RenderSetViewport(renderer, nullptr);
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize;
    if (ImGui::Begin("Spawn enemies", nullptr, flags)) {
      ImGui::Text("This is a message.");
    }

    ImGui::End();

    ImGui::Render();

    const auto &io = ImGui::GetIO();
    SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderSetScale(renderer, 1.0, 1.0);
  }
};
} // namespace Engine::ECS::System

#endif
