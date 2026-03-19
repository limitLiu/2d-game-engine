#include "ECS/Base/Registry.hpp"
#include "ECS/Base/System.hpp"

#include <ranges>

using namespace Engine::ECS;

Registry::Registry() { spdlog::info("Registry constructor called"); }

Registry::~Registry() { spdlog::info("Registry destructor called"); }

Entity Registry::createEntity() {
  i32 entityId;
  if (m_freeIDs.empty()) {
    entityId = m_entitiesCount++;
    if (entityId >= m_entityComponentSignatures.size()) {
      m_entityComponentSignatures.resize(entityId + 1);
    }
  } else {
    entityId = m_freeIDs.front();
    m_freeIDs.pop_front();
  }

  Entity entity(entityId);
  entity.m_registry = this;
  m_entities2BeAdded.insert(entity);
  spdlog::info("Entity created with id = {}", std::to_string(entityId));
  return entity;
}

void Registry::killEntity(const Entity entity) { m_entities2BeKilled.insert(entity); }

void Registry::addEntity2Systems(const Entity entity) const {
  const auto entityId = entity.id();
  const auto entityComponentSignature = m_entityComponentSignatures[entityId];
  for (const auto [_, snd] : systems()) {
    const auto &systemComponentSignature = snd->componentSignature();
    const bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
    if (isInterested) {
      snd->addEntity(entity);
    }
  }
}

void Registry::removeEntityFromSystems(const Entity entity) const {
  for (const auto &[_, snd] : systems()) {
    snd->removeEntity(entity);
  }
}

void Registry::update() {
  for (const auto entity : m_entities2BeAdded) {
    addEntity2Systems(entity);
  }
  m_entities2BeAdded.clear();
  for (const auto entity : m_entities2BeKilled) {
    removeEntityFromSystems(entity);
    m_entityComponentSignatures[entity.id()].reset();
    m_freeIDs.emplace_back(entity.id());
  }
  m_entities2BeKilled.clear();
}
