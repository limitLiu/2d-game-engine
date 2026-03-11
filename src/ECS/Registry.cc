#include "ECS/Base/Registry.hpp"
#include "ECS/Base/System.hpp"

#include <ranges>

using namespace ECS;

Registry::Registry() { spdlog::info("Registry constructor called"); }

Registry::~Registry() { spdlog::info("Registry destructor called"); }

Entity Registry::createEntity() {
  const auto entityId = _entitiesCount++;
  Entity entity(entityId);
  entity.m_registry = this;
  _entities2BeAdded.insert(entity);
  if (entityId >= _entityComponentSignatures.size()) {
    _entityComponentSignatures.resize(entityId + 1);
  }
  spdlog::info("Entity created with id = {}", std::to_string(entityId));
  return entity;
}

void Registry::addEntity2Systems(const Entity entity) const {
  const auto entityId = entity.id();
  const auto entityComponentSignature = _entityComponentSignatures[entityId];
  for (const auto &value : _systems | std::views::values) {
    const auto &systemComponentSignature = value->componentSignature();
    const bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
    if (isInterested) {
      value->addEntity(entity);
    }
  }
}

void Registry::update() {
  for (const auto entity : _entities2BeAdded) {
    addEntity2Systems(entity);
  }
  _entities2BeAdded.clear();
}
