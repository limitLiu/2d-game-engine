#include "ECS/Base/Registry.hpp"
#include "ECS/System/Base.hpp"

#include <spdlog/spdlog.h>

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
  entity.registry = this;
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
void Registry::setEntityTag(Entity entity, const std::string &tag) {
  m_entityPerTag.emplace(tag, entity);
  m_tagPerEntity.emplace(entity.id(), tag);
}

bool Registry::hasEntityTag(const Entity entity, const std::string &tag) const {
  if (!m_tagPerEntity.contains(entity.id())) {
    return false;
  }
  return m_entityPerTag.find(tag)->second == entity;
}

Entity Registry::entityFromTag(const std::string &tag) const { return m_entityPerTag.at(tag); }

void Registry::removeEntityTag(const Entity entity) {
  if (const auto taggedEntity = m_tagPerEntity.find(entity.id()); taggedEntity != m_tagPerEntity.end()) {
    m_entityPerTag.erase(taggedEntity->second);
    m_tagPerEntity.erase(taggedEntity);
  }
}

void Registry::setEntityGroup(Entity entity, const std::string &group) {
  m_entitiesPerGroup.emplace(group, std::set<Entity>());
  m_entitiesPerGroup[group].emplace(entity);
  m_groupPerEntities.emplace(entity.id(), group);
}

bool Registry::entityHasGroup(const Entity entity, const std::string &group) const {
  if (m_entitiesPerGroup.contains(group)) {
    return m_entitiesPerGroup.at(group).contains(entity);
  }
  return false;
}

std::vector<Entity> Registry::entityFromGroup(const std::string &group) const {
  const auto &entitiesFromGroup = m_entitiesPerGroup.at(group);
  return std::vector(entitiesFromGroup.begin(), entitiesFromGroup.end());
}

void Registry::removeEntityGroup(const Entity entity) {
  if (const auto groupedEntities = m_groupPerEntities.find(entity.id()); groupedEntities != m_groupPerEntities.end()) {
    if (const auto group = m_entitiesPerGroup.find(groupedEntities->second); group != m_entitiesPerGroup.end()) {
      if (const auto entityInGroup = group->second.find(entity); entityInGroup != group->second.end()) {
        group->second.erase(entityInGroup);
      }
    }
    m_groupPerEntities.erase(groupedEntities);
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

    for (auto pool : m_componentPools) {
      if (pool) {
        pool->removeEntityId(entity.id());
      }
    }

    m_freeIDs.emplace_back(entity.id());
    removeEntityTag(entity);
    removeEntityGroup(entity);
  }
  m_entities2BeKilled.clear();
}
