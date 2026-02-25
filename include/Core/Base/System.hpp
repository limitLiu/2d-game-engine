#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "Common.hpp"
#include "Core/Base/Component.hpp"
#include "Core/Base/Entity.hpp"
#include <vector>

namespace ecs {
struct System {
  System() = default;
  ~System() = default;

  void addEntity(Entity entity);

  void removeEntity(Entity entity);

  std::vector<Entity> entities() const;

  template <class T> void requireComponent();

  const Signature &componentSignature() const;

private:
  Signature _componentSignature;
  std::vector<Entity> _entities;
};

template <class T> void System::requireComponent() {
  const auto componentId = Component<T>::GetId();
  _componentSignature.set(componentId);
}
} // namespace ecs

#endif
