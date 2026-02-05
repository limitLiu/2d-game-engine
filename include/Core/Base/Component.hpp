#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "Common.hpp"

namespace ecs {
struct IComponent {
protected:
  static u32 nextId;
};

template <class T> struct Component : public IComponent {
  static u32 GetId() {
    static auto id = nextId++;
    return id;
  }
};
} // namespace ecs

#endif
