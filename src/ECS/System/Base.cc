#include "ECS/System/Base.hpp"

using namespace Engine::ECS;

void System::Base::addEntity(Entity entity) { m_entities.emplace_back(entity); }

void System::Base::removeEntity(Entity entity) {
  std::erase_if(m_entities, [&entity](const Entity rest) { return entity == rest; });
}

const Signature &System::Base::componentSignature() const { return m_componentSignature; }

std::vector<Entity> System::Base::entities() const { return m_entities; }