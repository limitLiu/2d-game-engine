#ifndef KEYBOARD_CONTROL_SYSTEM_HPP
#define KEYBOARD_CONTROL_SYSTEM_HPP

#include "ECS/Component/KeyboardControlled.hpp"
#include "ECS/Component/RigidBody.hpp"
#include "ECS/Component/Sprite.hpp"
#include "ECS/System/Base.hpp"
#include "Event/Bus.hpp"
#include "Event/Keypressed.hpp"

namespace Engine::ECS::System {
struct KeyboardControl : public Base {
  KeyboardControl() {
    requireComponent<Component::KeyboardControlled>();
    requireComponent<Component::Sprite>();
    requireComponent<Component::RigidBody>();
  }

  void subscribe(const std::unique_ptr<Event::Bus> &eventBus) {
    eventBus->subscribe<Event::KeyPressed>(this, &KeyboardControl::onKeyPressed);
  }

  void onKeyPressed(const Event::KeyPressed &event) const {
    for (auto entity : entities()) {
      const auto &keyboardControl = entity.component<Component::KeyboardControlled>();
      auto &sprite = entity.component<Component::Sprite>();
      auto &rigidBody = entity.component<Component::RigidBody>();
      switch (event.symbol) {
      case SDLK_w:
        rigidBody.velocity = keyboardControl.upVelocity;
        sprite.srcRect.y = sprite.height * 0;
        break;
      case SDLK_d:
        rigidBody.velocity = keyboardControl.rightVelocity;
        sprite.srcRect.y = sprite.height * 1;
        break;
      case SDLK_s:
        rigidBody.velocity = keyboardControl.downVelocity;
        sprite.srcRect.y = sprite.height * 2;
        break;
      case SDLK_a:
        rigidBody.velocity = keyboardControl.leftVelocity;
        sprite.srcRect.y = sprite.height * 3;
        break;
      default:
        break;
      }
    }
  }
};
} // namespace Engine::ECS::System

#endif
