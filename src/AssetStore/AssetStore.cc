#include "AssetStore/AssetStore.hpp"

#include <SDL_image.h>
#include <ranges>
#include <spdlog/spdlog.h>

using namespace Engine;

AssetStore::AssetStore() { spdlog::info("AssetStore constructor be called;"); }

AssetStore::~AssetStore() { spdlog::info("AssetStore destructor be called;"); }

auto AssetStore::addTexture(SDL_Renderer *renderer, const std::string &assetID, const std::string &path) -> void {
  auto *surface = IMG_Load(path.c_str());
  auto *const img = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  m_textures.emplace(assetID, img);
  spdlog::info("New texture added to the Asset Store with id = {}", assetID);
}

auto AssetStore::addFont(const std::string &assetID, const std::string &path, const i32 ptSize) -> void {
  m_fonts.emplace(assetID, TTF_OpenFont(path.c_str(), ptSize));
}

SDL_Texture *AssetStore::texture(const std::string &assetID) const { return m_textures.at(assetID); }

auto AssetStore::font(const std::string &assetID) const -> TTF_Font * { return m_fonts.at(assetID); }

auto AssetStore::clearTextures() -> void {
  for (const auto [_, texture] : m_textures) {
    SDL_DestroyTexture(texture);
  }
  m_textures.clear();

  for (auto *const font : m_fonts | std::views::values) {
    TTF_CloseFont(font);
  }
  m_fonts.clear();
}
