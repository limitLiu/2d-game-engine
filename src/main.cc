#include <iostream>

#include <SDL.h>

#include <glm/glm.hpp>

#include <sol/sol.hpp>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"

#include <spdlog/spdlog.h>

auto main(int argc, char *argv[]) -> int {
  spdlog::info("Starting ECS Engine...");

  sol::state lua;
  lua.open_libraries(sol::lib::base);
  lua.script("print('Lua is running via Sol2!')");

  glm::vec3 pos(1.0f, 2.0f, 3.0f);

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    spdlog::error("SDL Error: {}", SDL_GetError());
    return -1;
  }

  SDL_Quit();
  return 0;
}
