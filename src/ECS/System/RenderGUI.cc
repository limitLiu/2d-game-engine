#include "ECS/System/RenderGUI.hpp"
#include "Common.hpp"
#include "ECS/Base/Registry.hpp"
#include "ECS/Component/BoxCollider.hpp"
#include "ECS/Component/Health.hpp"
#include "ECS/Component/ProjectileEmitter.hpp"
#include "ECS/Component/RigidBody.hpp"
#include "ECS/Component/Sprite.hpp"
#include "ECS/Component/Transform.hpp"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

using namespace Engine::ECS;

auto System::RenderGUI::update(SDL_Renderer *renderer, const std::unique_ptr<Registry> &registry) const -> void {
  SDL_RenderSetViewport(renderer, nullptr);
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  if (ImGui::Begin("Spawn enemies")) {
    static i32 enemyPositionX = 0;
    static i32 enemyPositionY = 0;
    ImGui::InputInt("x position", &enemyPositionX);
    ImGui::InputInt("y position", &enemyPositionY);

    if (ImGui::Button("Create new enemy")) {
      Entity entity = registry->createEntity();
      entity.setGroup("enemies");
      entity.addComponent<ECS::Component::Transform>(glm::vec2(enemyPositionX, enemyPositionY), glm::vec2(1.0, 1.0),
                                                     0.0);
      entity.addComponent<ECS::Component::RigidBody>(glm::vec2(10.0, 0.0));
      entity.addComponent<ECS::Component::Sprite>("tank-panther-right", 32, 32, 1);
      entity.addComponent<ECS::Component::BoxCollider>(32, 32);
      entity.addComponent<ECS::Component::ProjectileEmitter>(glm::vec2(100.0, 0), 5000, 3000, 10, false);
      entity.addComponent<ECS::Component::Health>(100);
    }
  }

  ImGui::End();

  ImGui::Render();

  const auto &io = ImGui::GetIO();
  SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
  SDL_RenderSetScale(renderer, 1.0, 1.0);
}
