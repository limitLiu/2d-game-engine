#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "Common.hpp"
#include "Core/Base/Entity.hpp"
#include <vector>

namespace ecs {
struct System {
  System() = default;
  ~System() = default;

  void addEntity(Entity entity);

  void removeEntity(Entity entity);

  std::vector<Entity> entities() const;

private:
  Signature _componentSignature;
  std::vector<Entity> _entities;
};
} // namespace ecs

#endif
