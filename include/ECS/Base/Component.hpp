#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "Common.hpp"

namespace Engine::ECS {
struct IComponent {
protected:
  static u32 s_nextId;
};

template <class T> struct Component : public IComponent {
  static u32 GetId() {
    static auto id = s_nextId++;
    return id;
  }
};
} // namespace Engine::ECS

#endif
