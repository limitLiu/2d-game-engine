#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "Common.hpp"
#include "Core/Base/Component.hpp"
#include "Core/Base/Entity.hpp"
#include "Core/Base/Pool.hpp"
#include <memory>
#include <set>
#include <spdlog/spdlog.h>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace ecs {
typedef struct System System;

struct Registry {
  Registry();

  ~Registry();

  void update();

  Entity createEntity();

  template <class T, class... Args> void addComponent(Entity entity, Args &&...args);

  template <class T> void removeComponent(Entity entity);

  template <class T> bool hasComponent(Entity entity) const;

  template <class T, class... Args> void addSystem(Args &&...args);

  template <class T> void removeSystem();

  template <class T> bool hasSystem() const;

  template <class T> T &getSystem() const;

  void addEntity2Systems(Entity entity);

private:
  u32 _entitiesCount = 0;

  std::vector<std::shared_ptr<IPool>> _componentPools;

  std::vector<Signature> _entityComponentSignatures;

  std::unordered_map<std::type_index, std::shared_ptr<System>> _systems;

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
    auto pool = std::make_shared<Pool<T>>();
    _componentPools[componentId] = pool;
  }

  std::shared_ptr<Pool<T>> pool = std::static_pointer_cast<Pool<T>>(_componentPools[componentId]);

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

template <class T> bool Registry::hasComponent(Entity entity) const {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.id();
  return _entityComponentSignatures[entityId].test(componentId);
}

template <class T, class... Args> void Registry::addSystem(Args &&...args) {
  std::shared_ptr<T> newSystem = std::make_shared<T>(std::forward<Args>(args)...);
  _systems.insert(std::make_pair(std::type_index(typeid(T)), newSystem));
}

template <class T> void Registry::removeSystem() {
  const auto system = _systems.find(std::type_index(typeid(T)));
  _systems.erase(system);
}

template <class T> bool Registry::hasSystem() const {
  const auto system = _systems.find(std::type_index(typeid(T)));
  return system != _systems.end();
}

template <class T> T &Registry::getSystem() const {
  const auto system = _systems.find(std::type_index(typeid(T)));
  return *(std::static_pointer_cast<T>(system->second));
}
} // namespace ecs

#endif
