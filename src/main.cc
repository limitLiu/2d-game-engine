#include "Core/Game.hpp"

#include <SDL.h>
#include <spdlog/spdlog.h>

auto main(int argc, char *argv[]) -> int {
  engine::Game game;
  game.initialize();
  game.run();

  return 0;
}
