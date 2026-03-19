#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "Common.hpp"
#include "ECS/Base/Component.hpp"
#include "ECS/Base/Entity.hpp"
#include <vector>

namespace Engine::ECS {
struct System {
  System() = default;
  ~System() = default;

  void addEntity(Entity entity);

  void removeEntity(Entity entity);

  std::vector<Entity> entities() const;

  template <class T> void requireComponent();

  const Signature &componentSignature() const;

private:
  Signature m_componentSignature;
  std::vector<Entity> m_entities;
};

template <class T> void System::requireComponent() {
  const auto componentId = Component<T>::GetId();
  m_componentSignature.set(componentId);
}
} // namespace Engine::ECS

#endif
