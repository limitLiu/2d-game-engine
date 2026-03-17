#include "ECS/Base/Entity.hpp"
#include "ECS/Base/Registry.hpp"

using namespace ECS;

u32 Entity::id() const { return m_id; }

void Entity::kill() const { m_registry->killEntity(*this); }

bool Entity::operator==(const Entity &rhs) const { return id() == rhs.id(); }

bool Entity::operator!=(const Entity &rhs) const { return id() != rhs.id(); }

bool Entity::operator>(const Entity &rhs) const { return id() > rhs.id(); }

bool Entity::operator<(const Entity &rhs) const { return id() < rhs.id(); }

bool Entity::operator>=(const Entity &rhs) const { return id() >= rhs.id(); }

bool Entity::operator<=(const Entity &rhs) const { return id() <= rhs.id(); }
