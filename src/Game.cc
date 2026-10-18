#include "Game.hpp"
#include "AssetStore/AssetStore.hpp"
#include "ECS/Base/Registry.hpp"
#include "ECS/Component/Animation.hpp"
#include "ECS/Component/BoxCollider.hpp"
#include "ECS/Component/CameraFollow.hpp"
#include "ECS/Component/Health.hpp"
#include "ECS/Component/KeyboardControlled.hpp"
#include "ECS/Component/ProjectileEmitter.hpp"
#include "ECS/Component/RigidBody.hpp"
#include "ECS/Component/Sprite.hpp"
#include "ECS/Component/TextLabel.hpp"
#include "ECS/Component/Transform.hpp"
#include "ECS/System/Animation.hpp"
#include "ECS/System/CameraMovement.hpp"
#include "ECS/System/Collision.hpp"
#include "ECS/System/Damage.hpp"
#include "ECS/System/HealthBar.hpp"
#include "ECS/System/KeyboardControl.hpp"
#include "ECS/System/Movement.hpp"
#include "ECS/System/ProjectileEmitter.hpp"
#include "ECS/System/ProjectileLifecycle.hpp"
#include "ECS/System/Render.hpp"
#include "ECS/System/RenderCollider.hpp"
#include "ECS/System/RenderText.hpp"
#include "Event/Bus.hpp"
#include "Event/Keypressed.hpp"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <spdlog/spdlog.h>

using namespace Engine;

i32 Game::s_width = 960;
i32 Game::s_height = 540;
i32 Game::s_mapWidth;
i32 Game::s_mapHeight;

Game::Game() {
  m_registry = std::make_unique<ECS::Registry>();
  m_assetStore = std::make_unique<AssetStore>();
  m_bus = std::make_unique<Event::Bus>();
  m_camera = {.x = 0, .y = 0, .w = s_width, .h = s_height};
  spdlog::info("Game constructor called");
}

Game::~Game() { spdlog::info("Game destructor called"); }

void Game::initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    spdlog::error("SDL Error: {}", SDL_GetError());
    return;
  }
  if (TTF_Init() != 0) {
    spdlog::error("SDL TTF Error: {}", TTF_GetError());
    return;
  }

  m_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, s_width, s_height,
                              SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALLOW_HIGHDPI);
  if (m_window == nullptr) {
    spdlog::error("Failed to create SDL Window.");
    return;
  }
  m_ctx = SDL_CreateRenderer(m_window, -1, 0);
  if (m_ctx == nullptr) {
    spdlog::error("Failed to create SDL Window Renderer.");
    return;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  if (!ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_ctx)) {
    spdlog::error("Failed to initialize ImGui SDL2 backend");
    ImGui::DestroyContext();
    return;
  }
  if (!ImGui_ImplSDLRenderer2_Init(m_ctx)) {
    spdlog::error("Failed to initialize ImGui SDL renderer backend");
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    return;
  }

  SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);

  m_isRunning = true;
}

void Game::inputEvent() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
      m_isRunning = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        m_isRunning = false;
      }
      if (!ImGui::GetIO().WantCaptureKeyboard) {
        m_bus->emit<Event::KeyPressed>(event.key.keysym.sym);
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

  i32 outputW = 0;
  i32 outputH = 0;
  SDL_GetRendererOutputSize(m_ctx, &outputW, &outputH);
  const f32 gameScale = std::min(static_cast<f32>(outputW) / static_cast<f32>(s_width),
                                 static_cast<f32>(outputH) / static_cast<f32>(s_height));
  if (outputW > 0 && outputH > 0) {
    const SDL_Rect gameViewport{
        .x = static_cast<i32>((static_cast<f32>(outputW) / gameScale - static_cast<f32>(s_width)) / 2.0),
        .y = static_cast<i32>((static_cast<f32>(outputH) / gameScale - static_cast<f32>(s_height)) / 2.0),
        .w = s_width,
        .h = s_height,
    };
    SDL_RenderSetScale(m_ctx, gameScale, gameScale);
    SDL_RenderSetViewport(m_ctx, &gameViewport);
  }

  m_registry->system<ECS::System::Render>().update(m_ctx, m_assetStore, m_camera);
  m_registry->system<ECS::System::RenderText>().update(m_ctx, m_assetStore, m_camera);
  m_registry->system<ECS::System::HealthBar>().update(m_ctx, m_assetStore, m_camera);

#ifndef NDEBUG
  m_registry->system<ECS::System::RenderCollider>().update(m_ctx, m_camera);
  SDL_RenderSetViewport(m_ctx, nullptr);
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  ImGui::ShowDemoWindow();
  ImGui::Render();

  const auto &io = ImGui::GetIO();
  SDL_RenderSetScale(m_ctx, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_ctx);
  SDL_RenderSetScale(m_ctx, 1.0, 1.0);
#endif

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

void Game::loadLevel(i32 level) const {
  m_registry->addSystem<ECS::System::Movement>();
  m_registry->addSystem<ECS::System::Render>();
  m_registry->addSystem<ECS::System::Animation>();
  m_registry->addSystem<ECS::System::Collision>();
  m_registry->addSystem<ECS::System::RenderCollider>();
  m_registry->addSystem<ECS::System::Damage>();
  m_registry->addSystem<ECS::System::KeyboardControl>();
  m_registry->addSystem<ECS::System::CameraMovement>();
  m_registry->addSystem<ECS::System::ProjectileEmitter>();
  m_registry->addSystem<ECS::System::ProjectileLifecycle>();
  m_registry->addSystem<ECS::System::RenderText>();
  m_registry->addSystem<ECS::System::HealthBar>();

  m_assetStore->addTexture(m_ctx, "tank-panther-right", "assets/images/tank-panther-right.png");
  m_assetStore->addTexture(m_ctx, "truck-ford-up", "assets/images/truck-ford-up.png");
  m_assetStore->addTexture(m_ctx, "truck-ford-down", "assets/images/truck-ford-down.png");
  m_assetStore->addTexture(m_ctx, "jungle", "assets/tilemaps/jungle.png");
  m_assetStore->addTexture(m_ctx, "chopper", "assets/images/chopper-spritesheet.png");

  m_assetStore->addTexture(m_ctx, "radar", "assets/images/radar.png");
  m_assetStore->addTexture(m_ctx, "bullet", "assets/images/bullet.png");

  m_assetStore->addFont("charriot", "assets/fonts/charriot.ttf", 20);
  m_assetStore->addFont("pico8-5", "assets/fonts/pico8.ttf", 5);
  m_assetStore->addFont("pico8-10", "assets/fonts/pico8.ttf", 10);

  i32 tileSize = 32;
  constexpr i32 mapNumRows = 20;
  constexpr i32 mapNumCols = 25;
  constexpr f64 tileScale = 2.0;
  std::fstream mapFile;
  mapFile.open("assets/tilemaps/jungle.map");
  for (i32 y = 0; y < mapNumRows; ++y) {
    for (i32 x = 0; x < mapNumCols; ++x) {
      char ch;
      mapFile.get(ch);
      i32 srcRectY = std::atoi(&ch) * tileSize;
      mapFile.get(ch);
      i32 srcRectX = std::atoi(&ch) * tileSize;
      mapFile.ignore();
      ECS::Entity tile = m_registry->createEntity();
      tile.setGroup("tiles");
      tile.addComponent<ECS::Component::Transform>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)),
                                                   glm::vec2(tileScale, tileScale), 0);
      tile.addComponent<ECS::Component::Sprite>("jungle", tileSize, tileSize, 0, false, srcRectX, srcRectY);
    }
  }
  mapFile.close();
  s_mapWidth = mapNumCols * tileSize * tileScale;
  s_mapHeight = mapNumRows * tileSize * tileScale;

  ECS::Entity tank = m_registry->createEntity();
  tank.setGroup("enemies");
  tank.addComponent<ECS::Component::Transform>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
  tank.addComponent<ECS::Component::RigidBody>(glm::vec2(10.0, 0.0));
  tank.addComponent<ECS::Component::Sprite>("tank-panther-right", 32, 32, 1);
  tank.addComponent<ECS::Component::BoxCollider>(32, 32);
  tank.addComponent<ECS::Component::ProjectileEmitter>(glm::vec2(100.0, 0), 5000, 3000, 10, false);
  tank.addComponent<ECS::Component::Health>(100);

  ECS::Entity truck = m_registry->createEntity();

  truck.setGroup("enemies");
  truck.addComponent<ECS::Component::Transform>(glm::vec2(30.0, 60.0), glm::vec2(1.0, 1.0), 0.0);
  truck.addComponent<ECS::Component::RigidBody>(glm::vec2(0.0, 0.0));
  truck.addComponent<ECS::Component::Sprite>("truck-ford-up", 32, 32, 1);
  truck.addComponent<ECS::Component::BoxCollider>(32, 32);
  truck.addComponent<ECS::Component::ProjectileEmitter>(glm::vec2(0.0, 100.0), 2000, 5000, 10, false);
  truck.addComponent<ECS::Component::Health>(100);

  ECS::Entity chopper = m_registry->createEntity();
  chopper.setTag("player");
  chopper.addComponent<ECS::Component::Transform>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
  chopper.addComponent<ECS::Component::BoxCollider>(32.0, 32.0);
  chopper.addComponent<ECS::Component::RigidBody>(glm::vec2(0.0, 0.0));
  chopper.addComponent<ECS::Component::Sprite>("chopper", 32, 32, 1, false, 0, 32);
  chopper.addComponent<ECS::Component::Animation>(2, 12, true);
  chopper.addComponent<ECS::Component::KeyboardControlled>(glm::vec2(0, -80), glm::vec2(80, 0), glm::vec2(0, 80),
                                                           glm::vec2(-80, 0));
  chopper.addComponent<ECS::Component::CameraFollow>();
  chopper.addComponent<ECS::Component::ProjectileEmitter>(glm::vec2(150.0, 150.0), 0, 10000, 10, true);
  chopper.addComponent<ECS::Component::Health>(100);

  ECS::Entity radar = m_registry->createEntity();
  radar.addComponent<ECS::Component::Transform>(glm::vec2(s_width - 64 - 10, 10.0), glm::vec2(1.0, 1.0), 0.0);
  radar.addComponent<ECS::Component::RigidBody>(glm::vec2(00.0, 0.0));
  radar.addComponent<ECS::Component::Sprite>("radar", 64, 64, 2, true);
  radar.addComponent<ECS::Component::Animation>(8, 5, true);

  ECS::Entity label = m_registry->createEntity();
  label.addComponent<ECS::Component::TextLabel>(glm::vec2(s_width / 2, 10), "Show Text", "charriot",
                                                SDL_Color{.r = 255, .g = 255, .b = 255}, true);
}

void Game::setup() const { loadLevel(1); }

void Game::update() {
  if (const auto wait = MILLION_SECS_PER_FRAME - SDL_GetTicks() + m_millionSecsPreviousFrame;
      wait > 0 && wait <= MILLION_SECS_PER_FRAME) {
    SDL_Delay(wait);
  }
  const f64 deltaTime = (SDL_GetTicks() - m_millionSecsPreviousFrame) / 1000.0;
  m_millionSecsPreviousFrame = SDL_GetTicks();

  m_bus->reset();
  m_registry->system<ECS::System::Damage>().subscribe(m_bus);
  m_registry->system<ECS::System::KeyboardControl>().subscribe(m_bus);
  m_registry->system<ECS::System::ProjectileEmitter>().subscribe(m_bus);
  m_registry->update();
  m_registry->system<ECS::System::Movement>().update(deltaTime);
  m_registry->system<ECS::System::Animation>().update(deltaTime);
  m_registry->system<ECS::System::Collision>().update(m_bus);
  m_registry->system<ECS::System::ProjectileEmitter>().update(m_registry);
  m_registry->system<ECS::System::CameraMovement>().update(m_camera);
  m_registry->system<ECS::System::ProjectileLifecycle>().update();
}

void Game::destroy() const {
  if (ImGui::GetCurrentContext()) {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }
  m_assetStore->clearTextures();
  SDL_DestroyRenderer(m_ctx);
  SDL_DestroyWindow(m_window);
  TTF_Quit();
  SDL_Quit();
}
