#include "Core/Base/System.hpp"
#include <algorithm>

using namespace ecs;

void System::addEntity(Entity entity) { _entities.emplace_back(entity); }

void System::removeEntity(Entity entity) {
  _entities.erase(std::remove_if(_entities.begin(), _entities.end(), [&entity](Entity rest) { return entity == rest; }),
                  _entities.end());
}

const Signature &System::componentSignature() const { return _componentSignature; }

std::vector<Entity> System::entities() const { return _entities; }
