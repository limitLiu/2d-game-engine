#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Common.hpp"

namespace Engine::ECS {
struct Entity {
  explicit Entity(const u32 id) : m_id(id) {}

  struct Registry *registry{nullptr};

  usize id() const;

  void kill() const;

  bool operator==(const Entity &rhs) const;
  bool operator!=(const Entity &rhs) const;
  bool operator>(const Entity &rhs) const;
  bool operator<(const Entity &rhs) const;
  bool operator>=(const Entity &rhs) const;
  bool operator<=(const Entity &rhs) const;

  void setTag(const std::string &tag) const;

  bool hasTag(const std::string &tag) const;

  void setGroup(const std::string &group) const;

  bool belongsToGroup(const std::string &group) const;

  template <class T, class... Args> void addComponent(Args &&...args);

  template <class T> void removeComponent() const;

  template <class T> bool hasComponent() const;

  template <class T> T &component() const;

private:
  usize m_id;
};
} // namespace Engine::ECS

#endif
