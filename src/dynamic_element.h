#ifndef DYNA_ELEM_H
#define DYNA_ELEM_H
#include "SDL2/SDL.h"
#include "typedefs.h"
#include <string>

class DynamicElement {
public:
  DynamicElement(std::string pathToImage, SDL_Renderer *renderer);
  DynamicElement(SDL_Surface *image, SDL_Renderer *renderer);
  ~DynamicElement();
  void move(const Point &position);
  void resize(const Size &size);
  Rect getRect() const;
  Point getPosition() const;
  Size getSize() const;

private:
  void load(std::string pathToImage);
  void load(SDL_Surface *image);
  void draw();
  Point _position;
  Size _size;
  SDL_Texture *_texture;
  SDL_Renderer *_renderer;
};

#endif