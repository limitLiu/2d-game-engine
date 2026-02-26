#include "Core/Base/Registry.hpp"
#include "Core/Base/System.hpp"

using namespace ecs;

Registry::Registry() { spdlog::info("Registry constructor called"); }

Registry::~Registry() { spdlog::info("Registry destructor called"); }

Entity Registry::createEntity() {
  auto entityId = _entitiesCount++;
  Entity entity(entityId);
  _entitiesToBeAdded.insert(entity);
  if (entityId >= _entityComponentSignatures.size()) {
    _entityComponentSignatures.resize(entityId + 1);
  }
  spdlog::info("Entity created with id = {}", std::to_string(entityId));
  return entity;
}

void Registry::addEntity2Systems(Entity entity) {
  const auto entityId = entity.id();
  const auto entityComponentSignature = _entityComponentSignatures[entityId];
  for (auto &system : _systems) {
    const auto &systemComponentSignature = system.second->componentSignature();
    bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
    if (isInterested) {
      system.second->addEntity(entity);
    }
  }
}

void Registry::update() {
  for (auto entity : _entitiesToBeAdded) {
    addEntity2Systems(entity);
  }
  _entitiesToBeAdded.clear();
}
