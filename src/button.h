#pragma once
#include <functional>

#include "button_texture.h"
#include "dynamic_element.h"
#include <SDL2/SDL.h>
#include <SDL_ttf.h>

class Button : public DynamicElement {
public:
  Button();
  ~Button();

  void setMouseOutTexture(std::string path);
  void setMouseOverTexture(std::string path);
  void setMouseDownTexture(std::string path);
  void setFont(std::string font_path, SDL_Color color, int font_size);
  void handleEvent(SDL_Event *e) override;
  void render(SDL_Renderer *_renderer) override;
  void setRenderer(SDL_Renderer *renderer);

  template <class Function, class... Args>
  void setMouseDownEvent(Function &&f, Args &&... args) {
    _mouseDownEvent =
        std::bind(std::forward<Function>(f), std::forward<Args>(args)...);
  }

  template <class Function, class... Args>
  void setMouseUpEvent(Function &&f, Args &&... args) {
    _mouseUpEvent =
        std::bind(std::forward<Function>(f), std::forward<Args>(args)...);
  }

  template <class Function, class... Args>
  void setMouseOverEvent(Function &&f, Args &&... args) {
    _mouseOverEvent =
        std::bind(std::forward<Function>(f), std::forward<Args>(args)...);
  }

private:
  ButtonTexture _texture;
  SDL_Renderer *_renderer;
  ButtonState _buttonState{ButtonState::kMouseOut};

  std::function<void()> _mouseDownEvent;
  std::function<void()> _mouseUpEvent;
  std::function<void()> _mouseOverEvent;
};