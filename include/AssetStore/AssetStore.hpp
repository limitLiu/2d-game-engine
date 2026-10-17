#ifndef ASSET_STORE_HPP
#define ASSET_STORE_HPP

#include "Common.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <string>

namespace Engine {
struct AssetStore {
  AssetStore();

  ~AssetStore();

  auto addTexture(SDL_Renderer *renderer, const std::string &assetID, const std::string &path) -> void;

  auto addFont(const std::string &assetID, const std::string &path, const i32 ptSize) -> void;

  auto texture(const std::string &assetID) const -> SDL_Texture *;

  auto font(const std::string &assetID) const -> TTF_Font *;

  auto clearTextures() -> void;

private:
  std::map<std::string, SDL_Texture *> m_textures;
  std::map<std::string, TTF_Font *> m_fonts;
};
} // namespace Engine

#endif
