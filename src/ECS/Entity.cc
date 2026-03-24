#include "ECS/Base/Entity.hpp"
#include "ECS/Base/Registry.hpp"

using namespace Engine::ECS;

u32 Entity::id() const { return m_id; }

void Entity::kill() const { registry->killEntity(*this); }

bool Entity::operator==(const Entity &rhs) const { return id() == rhs.id(); }

bool Entity::operator!=(const Entity &rhs) const { return id() != rhs.id(); }

bool Entity::operator>(const Entity &rhs) const { return id() > rhs.id(); }

bool Entity::operator<(const Entity &rhs) const { return id() < rhs.id(); }

bool Entity::operator>=(const Entity &rhs) const { return id() >= rhs.id(); }

bool Entity::operator<=(const Entity &rhs) const { return id() <= rhs.id(); }
