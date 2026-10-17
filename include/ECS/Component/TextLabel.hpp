#ifndef TEXT_LABEL_COMPONENT_HPP
#define TEXT_LABEL_COMPONENT_HPP

#include <SDL.h>
#include <glm/glm.hpp>
#include <string>

namespace Engine::ECS::Component {
struct TextLabel {
  glm::vec2 position;
  SDL_Color color;
  std::string text;
  std::string assetID;
  bool isFixed;

  explicit TextLabel(const glm::vec2 position = glm::vec2(0, 0), const std::string &text = "",
                     const std::string &assetID = "", const SDL_Color &color = {.r = 0, .g = 0, .b = 0},
                     const bool isFixed = true)
      : position(position), color(color), text(text), assetID(assetID), isFixed(isFixed) {}
};
} // namespace Engine::ECS::Component

#endif
