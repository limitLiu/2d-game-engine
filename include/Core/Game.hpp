#ifndef GAME_HPP
#define GAME_HPP

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;

namespace engine {

struct Game {
  Game() = default;
  ~Game() = default;

  void initialize();

  void run();

  void destroy();

  void inputEvent();

  void update();

  void render();

  void setup();

private:
  SDL_Window *_window;
  SDL_Renderer *_renderer;
  bool _isRunning{false};
};
} // namespace engine

#endif
