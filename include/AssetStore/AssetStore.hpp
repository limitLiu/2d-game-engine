#ifndef ASSET_STORE_HPP
#define ASSET_STORE_HPP

#include <SDL.h>
#include <map>
#include <string>

namespace Engine {
struct AssetStore {
  AssetStore();

  ~AssetStore();

  void addTexture(SDL_Renderer *renderer, const std::string &assetID, const std::string &path);

  SDL_Texture *textures(const std::string &assetID) const;

  void clearTextures();

private:
  std::map<std::string, SDL_Texture *> m_textures;
};
} // namespace Engine

#endif
