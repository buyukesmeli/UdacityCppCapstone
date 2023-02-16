#pragma once
#include <string>

#include "typedefs.h"
#include <SDL2/SDL.h>

class DynamicElement {
public:
  DynamicElement() {}
  ~DynamicElement() {}
  void move(const Point &position);
  void resize(const Size &size);
  void setText(std::string const &text) { _text = text; }
  std::string getText() const { return _text; }
  Rect getRect() const;
  Point getPosition() const;
  Size getSize() const;

  virtual void handleEvent(SDL_Event *e) {}
  virtual void render(SDL_Renderer *_renderer) {}

private:
  Point _position;
  Size _size;
  std::string _text{};
};
