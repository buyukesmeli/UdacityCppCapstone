#include "button.h"
#include <SDL_ttf.h>

Button::Button() { _texture = ButtonTexture(); }

Button::~Button() { _texture.free(); }

// Create button textures for different button states
void Button::setMouseOutTexture(std::string path) {
  _texture.loadMouseOutTexture(path, _renderer);
}

void Button::setMouseOverTexture(std::string path) {
  _texture.loadMouseOverTexture(path, _renderer);
}

void Button::setMouseDownTexture(std::string path) {
  _texture.loadMouseDownTexture(path, _renderer);
}

// Get font from path and create button text texutre
void Button::setFont(std::string font_path, SDL_Color color, int font_size) {
  auto *ttf_font = TTF_OpenFont(font_path.c_str(), font_size);
  auto *textSurface =
      TTF_RenderText_Blended(ttf_font, getText().c_str(), color);
  _texture.loadTextTexture(textSurface, _renderer);
  _texture.setTextSize({textSurface->w, textSurface->h});
  SDL_FreeSurface(textSurface);
  TTF_CloseFont(ttf_font);
}

void Button::setRenderer(SDL_Renderer *renderer) { _renderer = renderer; }

// Define button event behavior
void Button::handleEvent(SDL_Event *e) {
  if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN ||
      e->type == SDL_MOUSEBUTTONUP) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    bool inside = getRect().contains({x, y});

    if (!inside) {
      _buttonState = ButtonState::kMouseOut;
    } else {
      switch (e->type) {
      case SDL_MOUSEMOTION:
        _buttonState = ButtonState::kMouseOver;
        if (_mouseOverEvent != nullptr)
          _mouseOverEvent();
        break;
      case SDL_MOUSEBUTTONDOWN:
        _buttonState = ButtonState::kMouseDown;
        if (_mouseDownEvent != nullptr)
          _mouseDownEvent();
        break;
      case SDL_MOUSEBUTTONUP:
        _buttonState = ButtonState::kMouseOver;
        if (_mouseUpEvent != nullptr)
          _mouseUpEvent();
        break;
      }
    }
  }
}

// Define button render behavior
void Button::render(SDL_Renderer *_renderer) {
  Rect rect = getRect();
  _texture.render(rect, _renderer, _buttonState);
}
