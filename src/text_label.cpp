#include "text_label.h"
#include <stdexcept>

TextLabel::TextLabel() : _renderer(nullptr), _texture(nullptr) {}
TextLabel::~TextLabel() {
  // clear defined tecture and font
  if (_texture != nullptr) {
    SDL_DestroyTexture(_texture);
    _texture = nullptr;
  }
  if (_textSurface != nullptr) {
    SDL_FreeSurface(_textSurface);
    _textSurface = nullptr;
  }
  if (_ttf_font != nullptr) {
    TTF_CloseFont(_ttf_font);
    _ttf_font = nullptr;
  }
}

void TextLabel::setRenderer(SDL_Renderer *renderer) { _renderer = renderer; }

// Create font from file and set the color
void TextLabel::setFont(std::string font_path, int font_size, SDL_Color color) {
  _ttf_font = TTF_OpenFont(font_path.c_str(), font_size);

  if (_ttf_font == nullptr) {
    std::invalid_argument e("Error when load font: " + font_path + ":\n" +
                            TTF_GetError());
    throw e;
  }
  _color = color;
}
// define the render behavior
void TextLabel::render(SDL_Renderer *_renderer) {
  Rect rect = getRect();
  std::string text = getText();
  _textSurface = TTF_RenderText_Blended(_ttf_font, text.c_str(), _color);
  _texture = SDL_CreateTextureFromSurface(_renderer, _textSurface);
  SDL_Rect destination = {rect.position.x, rect.position.y, rect.size.width,
                          rect.size.height};
  SDL_RenderCopy(_renderer, _texture, NULL, &destination);
}