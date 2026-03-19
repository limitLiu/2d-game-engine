#include "AssetStore/AssetStore.hpp"

#include <SDL_image.h>
#include <ranges>
#include <spdlog/spdlog.h>

using namespace Engine;

AssetStore::AssetStore() { spdlog::info("AssetStore constructor be called;"); }

AssetStore::~AssetStore() { spdlog::info("AssetStore destructor be called;"); }

void AssetStore::addTexture(SDL_Renderer *renderer, const std::string &assetID, const std::string &path) {
  auto *surface = IMG_Load(path.c_str());
  auto *const img = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  m_textures.emplace(assetID, img);
  spdlog::info("New texture added to the Asset Store with id = {}", assetID);
}

SDL_Texture *AssetStore::textures(const std::string &assetID) const { return m_textures.at(assetID); }

void AssetStore::clearTextures() {
  for (const auto [_, texture] : m_textures) {
    SDL_DestroyTexture(texture);
  }
  m_textures.clear();
}
