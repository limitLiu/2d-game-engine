#ifndef KEY_PRESSED_EVENT_HPP
#define KEY_PRESSED_EVENT_HPP

#include "Event/Base.hpp"

namespace Engine::Event {
struct KeyPressed : public Base {
  SDL_Keycode symbol;
  explicit KeyPressed(const SDL_Keycode symbol) : symbol(symbol) {};
};
} // namespace Engine::Event

#endif
