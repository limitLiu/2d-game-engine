#ifndef GAME_HPP
#define GAME_HPP

namespace Engine::ECS {
struct Registry;
} // namespace Engine::ECS

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;

#include "Common.hpp"

#include <memory>

namespace Engine {
namespace Event {
struct Bus;
} // namespace Event

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

  void loadLevel(i32 level) const;

  void setup() const;

private:
  SDL_Window *m_window{nullptr};
  SDL_Renderer *m_ctx{nullptr};
  bool m_isRunning{false};
  i32 m_millionSecsPreviousFrame{0};
  std::unique_ptr<ECS::Registry> m_registry;
  std::unique_ptr<struct AssetStore> m_assetStore;
  std::unique_ptr<Event::Bus> m_bus;
};
} // namespace Engine

#endif
