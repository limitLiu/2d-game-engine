#include "ECS/Base/System.hpp"

using namespace Engine::ECS;

void System::addEntity(Entity entity) { m_entities.emplace_back(entity); }

void System::removeEntity(Entity entity) {
  std::erase_if(m_entities, [&entity](const Entity rest) { return entity == rest; });
}

const Signature &System::componentSignature() const { return m_componentSignature; }

std::vector<Entity> System::entities() const { return m_entities; }
