#include "dynamic_element.h"
#include <SDL2/SDL_image.h>

#include <stdexcept>

DynamicElement::DynamicElement(std::string pathToImage, SDL_Renderer *renderer)
    : _renderer(renderer) {
  _position = {0, 0};
  _size = {0, 0};
}

DynamicElement::DynamicElement(SDL_Surface *image, SDL_Renderer *renderer)
    : _renderer(renderer) {
  _position = {0, 0};
  _size = {0, 0};
  load(image);
}

void DynamicElement::draw() {
  SDL_Rect destination = {_position.x, _position.y, _size.width, _size.height};
  SDL_RenderCopy(_renderer, _texture, nullptr, &destination);
}

DynamicElement::~DynamicElement() { SDL_DestroyTexture(_texture); }

void DynamicElement::move(const Point &position) { _position = position; }

void DynamicElement::resize(const Size &size) { _size = size; }

Rect DynamicElement::getRect() const { return {_position, _size}; }

Point DynamicElement::getPosition() const { return _position; }

Size DynamicElement::getSize() const { return _size; }

void DynamicElement::load(std::string pathToImage) {
  SDL_Surface *image = IMG_Load(pathToImage.c_str());
  if (image == nullptr) {
    std::invalid_argument e("Error when load image: " + pathToImage + ":\n" +
                            SDL_GetError());
    throw e;
  }

  _texture = SDL_CreateTextureFromSurface(_renderer, image);
  SDL_FreeSurface(image);
}

void DynamicElement::load(SDL_Surface *image) {
  if (image == nullptr) {
    std::invalid_argument e("Invalid image");
    throw e;
  }

  _texture = SDL_CreateTextureFromSurface(_renderer, image);
}
