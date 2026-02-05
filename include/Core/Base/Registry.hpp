#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "Common.hpp"
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace ecs {
typedef struct IPool IPool;

typedef struct System System;

struct Registry {
  Registry() = default;

private:
  u32 _entitiesCount = 0;

  std::vector<IPool *> componentPools;

  std::vector<Signature> entityComponentSignatures;

  std::unordered_map<std::type_index, System *> systems;
};
} // namespace ecs

#endif
