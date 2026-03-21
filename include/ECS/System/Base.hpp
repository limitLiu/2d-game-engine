#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "Common.hpp"
#include "ECS/Base/Entity.hpp"
#include "ECS/Component/Base.hpp"
#include <vector>

namespace Engine::ECS::System {
struct Base {
  Base() = default;
  ~Base() = default;

  void addEntity(Entity entity);

  void removeEntity(Entity entity);

  std::vector<Entity> entities() const;

  template <class T> void requireComponent();

  const Signature &componentSignature() const;

private:
  Signature m_componentSignature;
  std::vector<Entity> m_entities;
};

template <class T> void Base::requireComponent() {
  const auto componentId = Engine::ECS::Component::Base<T>::GetId();
  m_componentSignature.set(componentId);
}
} // namespace Engine::ECS::System

#endif