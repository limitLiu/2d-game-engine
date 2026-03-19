#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "Common.hpp"
#include "ECS/Base/Component.hpp"
#include "ECS/Base/Entity.hpp"
#include "ECS/Base/Pool.hpp"

#include <deque>
#include <memory>
#include <set>
#include <spdlog/spdlog.h>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Engine::ECS {
struct Registry {
  Registry();

  ~Registry();

  void update();

  Entity createEntity();

  void killEntity(Entity entity);

  template <class T, class... Args> void addComponent(Entity entity, Args &&...args);

  template <class T> void removeComponent(Entity entity);

  template <class T> bool hasComponent(Entity entity) const;

  template <class T> T &component(Entity entity);

  template <class T, class... Args> void addSystem(Args &&...args);

  template <class T> void removeSystem();

  template <class T> bool hasSystem() const;

  template <class T> T &system() const;

  void addEntity2Systems(Entity entity) const;

  void removeEntityFromSystems(Entity entity) const;

  std::unordered_map<std::type_index, std::shared_ptr<struct System>> systems() const { return m_systems; }

private:
  u32 m_entitiesCount = 0;

  std::vector<std::shared_ptr<IPool>> m_componentPools;

  std::vector<Signature> m_entityComponentSignatures;

  std::unordered_map<std::type_index, std::shared_ptr<struct System>> m_systems;

  std::set<Entity> m_entities2BeAdded;

  std::set<Entity> m_entities2BeKilled;

  std::deque<i32> m_freeIDs;
};

template <class T, class... Args> void Registry::addComponent(const Entity entity, Args &&...args) {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.id();

  if (componentId >= m_componentPools.size()) {
    m_componentPools.resize(componentId + 1, nullptr);
  }

  if (m_componentPools[componentId] == nullptr) {
    auto pool = std::make_shared<Pool<T>>();
    m_componentPools[componentId] = pool;
  }

  std::shared_ptr<Pool<T>> pool = std::static_pointer_cast<Pool<T>>(m_componentPools[componentId]);

  if (entityId >= pool->size()) {
    pool->resize(m_entitiesCount);
  }
  T newComponent(std::forward<Args>(args)...);
  pool->set(entityId, newComponent);
  m_entityComponentSignatures[entityId].set(componentId);
  spdlog::info("Component id = {} was added to entity id = {}", componentId, entityId);
}

template <class T> void Registry::removeComponent(const Entity entity) {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.id();
  m_entityComponentSignatures[entityId].set(componentId, false);
  spdlog::info("Component id = {} was removed from entity id = {}", componentId, entityId);
}

template <class T> bool Registry::hasComponent(const Entity entity) const {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.id();
  return m_entityComponentSignatures[entityId].test(componentId);
}

template <class T, class... Args> void Registry::addSystem(Args &&...args) {
  std::shared_ptr<T> newSystem = std::make_shared<T>(std::forward<Args>(args)...);
  m_systems.insert(std::make_pair(std::type_index(typeid(T)), newSystem));
}

template <class T> void Registry::removeSystem() {
  const auto system = m_systems.find(std::type_index(typeid(T)));
  m_systems.erase(system);
}

template <class T> bool Registry::hasSystem() const {
  const auto system = m_systems.find(std::type_index(typeid(T)));
  return system != m_systems.end();
}

template <class T> T &Registry::system() const {
  const auto system = m_systems.find(std::type_index(typeid(T)));
  return *(std::static_pointer_cast<T>(system->second));
}

template <class T> T &Registry::component(const Entity entity) {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.id();
  auto pool = std::static_pointer_cast<Pool<T>>(m_componentPools[componentId]);
  return pool->get(entityId);
}

template <class T, class... Args> void Entity::addComponent(Args &&...args) {
  m_registry->addComponent<T>(*this, std::forward<Args>(args)...);
}

template <class T> void Entity::removeComponent() const { m_registry->removeComponent<T>(*this); }

template <class T> bool Entity::hasComponent() const { return m_registry->hasComponent<T>(*this); }

template <class T> T &Entity::component() const { return m_registry->component<T>(*this); }
} // namespace Engine::ECS

#endif
