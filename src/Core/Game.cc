#include "Core/Game.hpp"
#include "Common.hpp"

#include <SDL.h>
#include <spdlog/spdlog.h>

using namespace engine;

const i32 WIDTH = 960;
const i32 HEIGHT = 544;

void Game::initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    spdlog::error("SDL Error: {}", SDL_GetError());
    return;
  }
  _window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                             SDL_WINDOW_BORDERLESS);
  if (_window == nullptr) {
    spdlog::error("Failed to create SDL Window.");
    return;
  }
  _renderer = SDL_CreateRenderer(_window, -1, 0);
  if (_renderer == nullptr) {
    spdlog::error("Failed to create SDL Window Renderer.");
    return;
  }
  SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
  _isRunning = true;
}

void Game::inputEvent() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      _isRunning = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        _isRunning = false;
      }
      break;
    default:
      break;
    }
  }
}

void Game::render() {
  SDL_SetRenderDrawColor(_renderer, 21, 21, 21, 255);
  SDL_RenderClear(_renderer);

  SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
  SDL_Rect player = {10, 10, 20, 20};
  SDL_RenderFillRect(_renderer, &player);

  SDL_RenderPresent(_renderer);
}

void Game::run() {
  while (_isRunning) {
    inputEvent();
    update();
    render();
  }
}

void Game::setup() {}

void Game::update() {}

void Game::destroy() { SDL_Quit(); }
