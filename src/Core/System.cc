#include "Core/Base/System.hpp"
#include <algorithm>

using namespace ecs;

void System::addEntity(Entity entity) { _entities.emplace_back(entity); }

void System::removeEntity(Entity entity) {
  std::erase_if(_entities, [&entity](const Entity rest) { return entity == rest; });
}

const Signature &System::componentSignature() const { return _componentSignature; }

std::vector<Entity> System::entities() const { return _entities; }
