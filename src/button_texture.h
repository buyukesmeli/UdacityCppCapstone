#pragma once
#include <SDL.h>
#include <string>

#include "typedefs.h"

class ButtonTexture {
public:
  ButtonTexture();
  ~ButtonTexture();
  void loadMouseOutTexture(std::string path, SDL_Renderer *renderer);
  void loadMouseOverTexture(std::string path, SDL_Renderer *renderer);
  void loadMouseDownTexture(std::string path, SDL_Renderer *renderer);
  void loadTextTexture(SDL_Surface *image, SDL_Renderer *renderer);
  void free();
  void render(Rect rect, SDL_Renderer *renderer, ButtonState type);
  int getWidth();
  int getHeight();
  void resize(const Size &size);
  void setTextSize(Size size) { _textSize = size; }

private:
  SDL_Texture *_mouseOutTexture;
  SDL_Texture *_mouseOverTexture;
  SDL_Texture *_mouseDownTexture;
  SDL_Texture *_textTexture;
  Size _textSize;
  int _width;
  int _height;
};