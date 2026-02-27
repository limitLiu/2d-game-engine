#ifndef GAME_HPP
#define GAME_HPP

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;

#include "Common.hpp"
#include "Core/Base/Registry.hpp"

namespace engine {
constexpr i32 FPS = 60;
constexpr f32 MILLION_SECS_PER_FRAME = 1000 / FPS;

struct Game {
  Game();
  ~Game();

  void initialize();

  void run();

  void destroy();

  void inputEvent();

  void update();

  void render();

  void setup() const;

private:
  SDL_Window *_window{nullptr};
  SDL_Renderer *_ctx{nullptr};
  bool _isRunning{false};
  i32 _millionSecsPreviousFrame{0};
  std::unique_ptr<ecs::Registry> _registry;
};
} // namespace engine

#endif
