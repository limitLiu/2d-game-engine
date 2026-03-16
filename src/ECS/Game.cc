#include "ECS/Game.hpp"
#include "ECS/Base/Entity.hpp"
#include "ECS/Base/Registry.hpp"
#include "ECS/Components/AnimationComponent.hpp"
#include "ECS/Components/BoxColliderComponent.hpp"
#include "ECS/Components/RigidBodyComponent.hpp"
#include "ECS/Components/SpriteComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Systems/MovementSystem.hpp"
#include "ECS/Systems/RenderSystem.hpp"

#include "AssetStore/AssetStore.hpp"
#include "ECS/Systems/AnimationSystem.hpp"
#include "ECS/Systems/CollisionSystem.hpp"

#include <cstdlib>
#include <fstream>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

using namespace Engine;

constexpr i32 WIDTH = 960;
constexpr i32 HEIGHT = 540;

static glm::vec2 position;

Game::Game() {
  m_registry = std::make_unique<ECS::Registry>();
  m_assetStore = std::make_unique<AssetStore>();
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
  m_registry->system<ECS::RenderSystem>().update(m_ctx, m_assetStore);
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

void Game::loadLevel(i32 level) {
  m_registry->addSystem<ECS::MovementSystem>();
  m_registry->addSystem<ECS::RenderSystem>();
  m_registry->addSystem<ECS::AnimationSystem>();
  m_registry->addSystem<ECS::CollisionSystem>();

  m_assetStore->addTexture(m_ctx, "tank-panther-right", "assets/images/tank-panther-right.png");
  m_assetStore->addTexture(m_ctx, "truck-ford-up", "assets/images/truck-ford-up.png");
  m_assetStore->addTexture(m_ctx, "truck-ford-down", "assets/images/truck-ford-down.png");
  m_assetStore->addTexture(m_ctx, "jungle", "assets/tilemaps/jungle.png");
  m_assetStore->addTexture(m_ctx, "chopper", "assets/images/chopper.png");
  m_assetStore->addTexture(m_ctx, "radar", "assets/images/radar.png");

  i32 tileSize = 32;
  constexpr i32 mapNumRows = 20;
  std::fstream mapFile;
  mapFile.open("assets/tilemaps/jungle.map");
  for (i32 y = 0; y < mapNumRows; ++y) {
    constexpr i32 mapNumCols = 25;
    for (i32 x = 0; x < mapNumCols; ++x) {
      constexpr f64 tileScale = 2.0;
      char ch;
      mapFile.get(ch);
      i32 srcRectY = std::atoi(&ch) * tileSize;
      mapFile.get(ch);
      i32 srcRectX = std::atoi(&ch) * tileSize;
      mapFile.ignore();
      ECS::Entity tile = m_registry->createEntity();
      tile.addComponent<ECS::TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)),
                                                 glm::vec2(tileScale, tileScale), 0);
      tile.addComponent<ECS::SpriteComponent>("jungle", tileSize, tileSize, 0, srcRectX, srcRectY);
    }
  }
  mapFile.close();

  ECS::Entity tank = m_registry->createEntity();
  tank.addComponent<ECS::TransformComponent>(glm::vec2(10.0, 20.0), glm::vec2(1.0, 1.0), 0.0);
  tank.addComponent<ECS::RigidBodyComponent>(glm::vec2(10.0, 0.0));
  tank.addComponent<ECS::SpriteComponent>("tank-panther-right", 32, 32, 1);
  tank.addComponent<ECS::BoxColliderComponent>(32, 32);

  ECS::Entity truck = m_registry->createEntity();
  truck.addComponent<ECS::TransformComponent>(glm::vec2(30.0, 20.0), glm::vec2(1.0, 1.0), 0.0);
  truck.addComponent<ECS::RigidBodyComponent>(glm::vec2(0.0, 0.0));
  truck.addComponent<ECS::SpriteComponent>("truck-ford-up", 32, 32, 1);
  truck.addComponent<ECS::BoxColliderComponent>(32, 32);

  ECS::Entity chopper = m_registry->createEntity();
  chopper.addComponent<ECS::TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
  chopper.addComponent<ECS::RigidBodyComponent>(glm::vec2(20.0, 0.0));
  chopper.addComponent<ECS::SpriteComponent>("chopper", 32, 32, 1);
  chopper.addComponent<ECS::AnimationComponent>(2, 12, true);

  ECS::Entity radar = m_registry->createEntity();
  radar.addComponent<ECS::TransformComponent>(glm::vec2(WIDTH - 64 - 10, 10.0), glm::vec2(1.0, 1.0), 0.0);
  radar.addComponent<ECS::RigidBodyComponent>(glm::vec2(00.0, 0.0));
  radar.addComponent<ECS::SpriteComponent>("radar", 64, 64, 2);
  radar.addComponent<ECS::AnimationComponent>(8, 5, true);
}

void Game::setup() { loadLevel(1); }

void Game::update() {
  if (const auto wait = MILLION_SECS_PER_FRAME - SDL_GetTicks() + m_millionSecsPreviousFrame;
      wait > 0 && wait <= MILLION_SECS_PER_FRAME) {
    SDL_Delay(wait);
  }
  const f64 deltaTime = (SDL_GetTicks() - m_millionSecsPreviousFrame) / 1000.0;
  m_millionSecsPreviousFrame = SDL_GetTicks();
  m_registry->update();
  m_registry->system<ECS::MovementSystem>().update(deltaTime);
  m_registry->system<ECS::AnimationSystem>().update(deltaTime);
  m_registry->system<ECS::CollisionSystem>().update();
}

void Game::destroy() {
  SDL_DestroyRenderer(m_ctx);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}
