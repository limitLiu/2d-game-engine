#ifndef CAMERA_MOVEMENT_SYSTEM_HPP
#define CAMERA_MOVEMENT_SYSTEM_HPP

#include "ECS/Component/CameraFollow.hpp"
#include "ECS/Component/Transform.hpp"
#include "ECS/System/Base.hpp"

namespace Engine::ECS::System {
struct CameraMovement : public Base {
  CameraMovement() {
    requireComponent<Component::Transform>();
    requireComponent<Component::CameraFollow>();
  }

  void update(SDL_Rect &camera) const {
    for (const auto entity : entities()) {
      auto &cameraFollow = entity.component<Component::CameraFollow>();
      const auto &transform = entity.component<Component::Transform>();
      if (transform.position.x + (camera.w / 2) < Game::s_mapWidth) {
        camera.x = transform.position.x - (Game::s_width / 2);
      }
      if (transform.position.y + (camera.h / 2) < Game::s_mapHeight) {
        camera.y = transform.position.y - (Game::s_height / 2);
      }
      camera.x = camera.x < 0 ? 0 : camera.x;
      camera.y = camera.y < 0 ? 0 : camera.y;
      camera.x = camera.x > camera.w ? camera.w : camera.x;
      camera.y = camera.y > camera.h ? camera.h : camera.y;
    }
  }
};
} // namespace Engine::ECS::System

#endif
