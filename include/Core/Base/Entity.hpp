#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Common.hpp"

namespace ecs {
struct Entity {
  Entity(u32 id) : _id(id) {}

  u32 id() const { return _id; }

private:
  u32 _id;
};
} // namespace ecs

#endif
