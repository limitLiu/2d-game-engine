#ifndef GAME_HPP
#define GAME_HPP

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;

#include "Common.hpp"
#include "Core/Base/Registry.hpp"

namespace engine {
const i32 FPS = 60;
const f32 MILLISECS_PER_FRAME = 1000 / FPS;

struct Game {
  Game();
  ~Game();

  void initialize();

  void run();

  void destroy();

  void inputEvent();

  void update();

  void render();

  void setup();

private:
  SDL_Window *_window;
  SDL_Renderer *_ctx;
  bool _isRunning;
  i32 _millisecsPreviousFrame{0};
  std::unique_ptr<ecs::Registry> _registry;
};
} // namespace engine

#endif
