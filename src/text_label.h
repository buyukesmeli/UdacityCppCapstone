#pragma once
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "dynamic_element.h"
#include "typedefs.h"

class TextLabel : public DynamicElement {
public:
  TextLabel();
  ~TextLabel();
  void setFont(std::string font_path, int font_size, SDL_Color color);
  void render(SDL_Renderer *_renderer) override;
  void setRenderer(SDL_Renderer *renderer);

private:
  TTF_Font *_ttf_font;
  SDL_Surface *_textSurface;
  SDL_Renderer *_renderer;
  SDL_Texture *_texture;
  SDL_Color _color;
  std::string _text;
  Size _size;
};