#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "Common.hpp"

namespace Engine::ECS::Component {
struct IBase {
protected:
  static u32 s_nextId;
};

template <class T> struct Base : public IBase {
  static u32 GetId() {
    static auto id = s_nextId++;
    return id;
  }
};
} // namespace Engine::ECS::Component

#endif