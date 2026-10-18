#include "Game.hpp"

auto main(int argc, char *argv[]) -> int {
  Engine::Game game;
  game.initialize();
  game.run();
  game.destroy();

  return 0;
}
