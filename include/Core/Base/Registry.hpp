#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "Common.hpp"
#include "Core/Base/Component.hpp"
#include "Core/Base/Entity.hpp"
#include "Core/Base/Pool.hpp"
#include <set>
#include <spdlog/spdlog.h>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace ecs {
typedef struct System System;

struct Registry {
  Registry() = default;

  void update();

  Entity createEntity();

  template <class T, class... Args> void addComponent(Entity entity, Args &&...args);

  template <class T> void removeComponent(Entity entity);

  template <class T> bool hasComponent(Entity entity);

private:
  u32 _entitiesCount = 0;

  std::vector<IPool *> _componentPools;

  std::vector<Signature> _entityComponentSignatures;

  std::unordered_map<std::type_index, System *> _systems;

  std::set<Entity> _entitiesToBeAdded;
  std::set<Entity> _entitiesToBeKilled;
};

template <class T, class... Args> void Registry::addComponent(Entity entity, Args &&...args) {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.id();

  if (componentId >= _componentPools.size()) {
    _componentPools.resize(componentId + 1, nullptr);
  }

  if (_componentPools[componentId] == nullptr) {
    auto *pool = new Pool<T>();
    _componentPools[componentId] = pool;
  }

  Pool<T> *pool = _componentPools[componentId];

  if (entityId >= pool->size()) {
    pool->resize(_entitiesCount);
  }
  T newComponent(std::forward<Args>(args)...);
  pool->set(entityId, newComponent);
  _entityComponentSignatures[entityId].set(componentId);
}

template <class T> void Registry::removeComponent(Entity entity) {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.id();
  _entityComponentSignatures[entityId].set(componentId, false);
}

template <class T> bool Registry::hasComponent(Entity entity) {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.id();
  return _entityComponentSignatures[entityId].test(componentId);
}

} // namespace ecs

#endif
