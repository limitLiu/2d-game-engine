#include "ECS/Base/Entity.hpp"
#include "ECS/Base/Registry.hpp"

using namespace Engine::ECS;

usize Entity::id() const { return m_id; }

void Entity::kill() const { registry->killEntity(*this); }

bool Entity::operator==(const Entity &rhs) const { return id() == rhs.id(); }

bool Entity::operator!=(const Entity &rhs) const { return id() != rhs.id(); }

bool Entity::operator>(const Entity &rhs) const { return id() > rhs.id(); }

bool Entity::operator<(const Entity &rhs) const { return id() < rhs.id(); }

bool Entity::operator>=(const Entity &rhs) const { return id() >= rhs.id(); }

bool Entity::operator<=(const Entity &rhs) const { return id() <= rhs.id(); }

void Entity::setTag(const std::string &tag) const { registry->setEntityTag(*this, tag); }

bool Entity::hasTag(const std::string &tag) const { return registry->hasEntityTag(*this, tag); }

void Entity::setGroup(const std::string &group) const { registry->setEntityGroup(*this, group); }

bool Entity::belongsToGroup(const std::string &group) const { return registry->entityHasGroup(*this, group); }
