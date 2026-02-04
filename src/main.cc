#include "Core/Game.hpp"

auto main(int argc, char *argv[]) -> int {
  engine::Game game;
  game.initialize();
  game.run();

  return 0;
}
