#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Common.hpp"

namespace ecs {
struct Entity {
  explicit Entity(const u32 id) : _id(id) {}

  struct Registry *_registry{nullptr};

  u32 id() const;

  bool operator==(const Entity &rhs) const;
  bool operator!=(const Entity &rhs) const;
  bool operator>(const Entity &rhs) const;
  bool operator<(const Entity &rhs) const;
  bool operator>=(const Entity &rhs) const;
  bool operator<=(const Entity &rhs) const;

  template <class T, class... Args> void addComponent(Args &&...args);

  template <class T> void removeComponent() const;

  template <class T> bool hasComponent() const;

  template <class T> T &component() const;

private:
  u32 _id;
};
} // namespace ecs

#endif
