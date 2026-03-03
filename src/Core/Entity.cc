#include "Core/Base/Entity.hpp"

using namespace ecs;

u32 Entity::id() const { return m_id; }

bool Entity::operator==(const Entity &rhs) const { return id() == rhs.id(); }

bool Entity::operator!=(const Entity &rhs) const { return id() != rhs.id(); }

bool Entity::operator>(const Entity &rhs) const { return id() > rhs.id(); }

bool Entity::operator<(const Entity &rhs) const { return id() < rhs.id(); }

bool Entity::operator>=(const Entity &rhs) const { return id() >= rhs.id(); }

bool Entity::operator<=(const Entity &rhs) const { return id() <= rhs.id(); }
