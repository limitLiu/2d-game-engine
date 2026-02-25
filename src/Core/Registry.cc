#include "Core/Base/Registry.hpp"

using namespace ecs;

Entity Registry::createEntity() {
  auto entityId = _entitiesCount++;
  Entity entity(entityId);
  _entitiesToBeAdded.insert(entity);
  spdlog::info("Entity created with id = ", std::to_string(entityId));
  return entity;
}

void Registry::update() {}
