#include "Core/Game.hpp"
#include "Core/Base/Entity.hpp"
#include "Core/Components/RigidBodyComponent.hpp"
#include "Core/Components/SpriteComponent.hpp"
#include "Core/Components/TransformComponent.hpp"
#include "Core/Systems/MovementSystem.hpp"
#include "Core/Systems/RenderSystem.hpp"

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

using namespace engine;

constexpr i32 WIDTH = 960;
constexpr i32 HEIGHT = 540;

static glm::vec2 position;

Game::Game() {
  m_registry = std::make_unique<ecs::Registry>();
  spdlog::info("Game constructor called");
}

Game::~Game() { spdlog::info("Game destructor called"); }

void Game::initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    spdlog::error("SDL Error: {}", SDL_GetError());
    return;
  }
  m_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                              SDL_WINDOW_BORDERLESS);
  if (m_window == nullptr) {
    spdlog::error("Failed to create SDL Window.");
    return;
  }
  m_ctx = SDL_CreateRenderer(m_window, -1, 0);
  if (m_ctx == nullptr) {
    spdlog::error("Failed to create SDL Window Renderer.");
    return;
  }
  SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
  m_isRunning = true;
}

void Game::inputEvent() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      m_isRunning = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        m_isRunning = false;
      }
      break;
    default:
      break;
    }
  }
}

void Game::render() {
  SDL_SetRenderDrawColor(m_ctx, 0, 0, 0, 0);
  SDL_RenderClear(m_ctx);
  m_registry->system<ecs::RenderSystem>().update(m_ctx);
  SDL_RenderPresent(m_ctx);
}

void Game::run() {
  setup();
  while (m_isRunning) {
    inputEvent();
    update();
    render();
  }
}

void Game::setup() const {
  m_registry->addSystem<ecs::MovementSystem>();
  m_registry->addSystem<ecs::RenderSystem>();

  ecs::Entity tank = m_registry->createEntity();
  tank.addComponent<ecs::TransformComponent>(glm::vec2(10.0, 20.0), glm::vec2(1.0, 1.0), 0.0);
  tank.addComponent<ecs::RigidBodyComponent>(glm::vec2(20.0, 0.0));
  tank.addComponent<ecs::SpriteComponent>(32, 32);
}

void Game::update() {
  if (const auto wait = MILLION_SECS_PER_FRAME - SDL_GetTicks() + m_millionSecsPreviousFrame;
      wait > 0 && wait <= MILLION_SECS_PER_FRAME) {
    SDL_Delay(wait);
  }
  const f64 deltaTime = (SDL_GetTicks() - m_millionSecsPreviousFrame) / 1000.0;
  m_millionSecsPreviousFrame = SDL_GetTicks();
  m_registry->update();
  m_registry->system<ecs::MovementSystem>().update(deltaTime);
}

void Game::destroy() {
  SDL_DestroyRenderer(m_ctx);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}
