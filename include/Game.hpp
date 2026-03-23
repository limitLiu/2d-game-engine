#ifndef GAME_HPP
#define GAME_HPP

namespace Engine::ECS {
struct Registry;
} // namespace Engine::ECS

#include "Common.hpp"

#include <SDL.h>
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

  void destroy() const;

  void inputEvent();

  void update();

  void render();

  void loadLevel(i32 level) const;

  void setup() const;

  static i32 s_width;

  static i32 s_height;

  static i32 s_mapWidth;

  static i32 s_mapHeight;

private:
  SDL_Window *m_window{nullptr};
  SDL_Renderer *m_ctx{nullptr};
  bool m_isRunning{false};
  i32 m_millionSecsPreviousFrame{0};
  std::unique_ptr<ECS::Registry> m_registry;
  std::unique_ptr<struct AssetStore> m_assetStore;
  std::unique_ptr<Event::Bus> m_bus;
  SDL_Rect m_camera;
};
} // namespace Engine

#endif
