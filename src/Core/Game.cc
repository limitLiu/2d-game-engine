#include "Core/Game.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

using namespace engine;

const i32 WIDTH = 960;
const i32 HEIGHT = 540;

static glm::vec2 position;
static glm::vec2 velocity;

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
  _ctx = SDL_CreateRenderer(_window, -1, 0);
  if (_ctx == nullptr) {
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
  SDL_SetRenderDrawColor(_ctx, 0, 0, 0, 0);
  SDL_RenderClear(_ctx);

  auto *surface = IMG_Load("assets/images/tank-tiger-right.png");
  auto *texture = SDL_CreateTextureFromSurface(_ctx, surface);
  SDL_FreeSurface(surface);

  SDL_Rect dstRect = {static_cast<i32>(position.x), static_cast<i32>(position.y), 32, 32};
  SDL_RenderCopy(_ctx, texture, nullptr, &dstRect);
  SDL_DestroyTexture(texture);

  SDL_RenderPresent(_ctx);
}

void Game::run() {
  setup();
  while (_isRunning) {
    inputEvent();
    update();
    render();
  }
}

void Game::setup() {
  position = glm::vec2(10.0, 20.0);
  velocity = glm::vec2(20.0, 0.0);
}

void Game::update() {
  auto wait = MILLISECS_PER_FRAME - SDL_GetTicks() + _millisecsPreviousFrame;
  if (wait > 0 && wait <= MILLISECS_PER_FRAME) {
    SDL_Delay(wait);
  }
  f64 deltaTime = (SDL_GetTicks() - _millisecsPreviousFrame) / 1000.0;

  _millisecsPreviousFrame = SDL_GetTicks();
  position.x += velocity.x * deltaTime;
  position.y += velocity.y * deltaTime;
}

void Game::destroy() { SDL_Quit(); }
