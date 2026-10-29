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

auto System::RenderGUI::update(SDL_Renderer *renderer, const std::unique_ptr<Registry> &registry,
                               const SDL_Rect &camera) const -> void {
  SDL_RenderSetViewport(renderer, nullptr);
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  if (ImGui::Begin("Spawn enemies")) {
    static i32 positionX = 0;
    static i32 positionY = 0;
    static i32 scaleX = 1;
    static i32 scaleY = 1;
    static i32 velocityX = 0;
    static i32 velocityY = 0;
    static i32 health = 100;
    static f32 rotation = 0.0;
    static f32 projectileAngle = 0.0;
    static f32 projectileSpeed = 100.0;
    static i32 projectileRepeat = 10;
    static i32 projectileDuration = 10;
    const char *sprites[] = {"tank-panther-right", "truck-ford-up"};
    static i32 selectedIndex = 0;

    if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::Combo("texture id", &selectedIndex, sprites, IM_ARRAYSIZE(sprites));
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::InputInt("position x", &positionX);
      ImGui::InputInt("position y", &positionY);
      ImGui::SliderInt("scale x", &scaleX, 1, 10);
      ImGui::SliderInt("scale y", &scaleY, 1, 10);
      ImGui::SliderAngle("rotation (deg)", &rotation, 0, 360);
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Rigid body", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::InputInt("velocity x", &velocityX);
      ImGui::InputInt("velocity y", &velocityY);
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Projectile emitter", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::SliderAngle("angle (deg)", &projectileAngle, 0, 360);
      ImGui::SliderFloat("speed (px/sec)", &projectileSpeed, 10, 500);
      ImGui::InputInt("repeat (sec)", &projectileRepeat);
      ImGui::InputInt("duration (sec)", &projectileDuration);
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::SliderInt("%", &health, 0, 100);
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Create new enemy")) {
      Entity entity = registry->createEntity();
      entity.setGroup("enemies");
      entity.addComponent<Component::Transform>(glm::vec2(positionX, positionY), glm::vec2(scaleX, scaleY),
                                                glm::degrees(rotation));
      entity.addComponent<Component::RigidBody>(glm::vec2(velocityX, velocityY));
      entity.addComponent<Component::Sprite>(sprites[selectedIndex], 32, 32, 2);
      entity.addComponent<Component::BoxCollider>(25, 20, glm::vec2(5, 5));
      const auto projectileVelocityX = cos(projectileAngle) * projectileSpeed;
      const auto projectileVelocityY = sin(projectileAngle) * projectileSpeed;
      entity.addComponent<Component::ProjectileEmitter>(glm::vec2(projectileVelocityX, projectileVelocityY),
                                                        projectileRepeat * 1000, projectileDuration * 1000, 10, false);
      entity.addComponent<Component::Health>(health);

      positionX = positionY = rotation = projectileAngle = 0;
      scaleX = scaleY = 1;
      projectileRepeat = projectileDuration = 10;
      projectileSpeed = 100;
      health = 100;
    }
  }
  ImGui::End();

  constexpr auto flag = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
  ImGui::SetNextWindowPos(ImVec2(camera.w * 0.5, camera.h), ImGuiCond_Always, ImVec2(0, 0));
  ImGui::SetNextWindowBgAlpha(0.9f);
  if (ImGui::Begin("Map Coordinates", nullptr, flag)) {
    ImGui::Text("Map Coordinates (x = %.1f, y = %.1f)", ImGui::GetIO().MousePos.x + camera.x,
                ImGui::GetIO().MousePos.y + camera.y);
  }
  ImGui::End();

  ImGui::Render();

  const auto &io = ImGui::GetIO();
  SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
  SDL_RenderSetScale(renderer, 1.0, 1.0);
}
