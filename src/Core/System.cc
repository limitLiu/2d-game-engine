#include "Core/Base/System.hpp"

using namespace ecs;

void System::removeEntity(Entity entity) {}

void System::addEntity(Entity entity) {}

std::vector<Entity> System::entities() const { return _entities; }
