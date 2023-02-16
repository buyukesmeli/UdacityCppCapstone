#include <stdexcept>

#include <SDL2/SDL_image.h>

#include "button_texture.h"

ButtonTexture::ButtonTexture() {
  _mouseOutTexture = NULL;
  _mouseOverTexture = NULL;
  _mouseDownTexture = NULL;
  _width = 0;
  _height = 0;
}

ButtonTexture::~ButtonTexture() { free(); }

void ButtonTexture::free() {
  // Fee allocated textures
  if (_mouseOutTexture != NULL) {
    SDL_DestroyTexture(_mouseOutTexture);
    _mouseOutTexture = NULL;
  }
  if (_mouseOverTexture != NULL) {
    SDL_DestroyTexture(_mouseOverTexture);
    _mouseOverTexture = NULL;
  }
  if (_mouseDownTexture != NULL) {
    SDL_DestroyTexture(_mouseDownTexture);
    _mouseDownTexture = NULL;
  }
}

// Create the texture from the image path when the button is not clicked
void ButtonTexture::loadMouseOutTexture(std::string path,
                                        SDL_Renderer *renderer) {
  SDL_Surface *image = IMG_Load(path.c_str());
  if (image == nullptr) {
    std::invalid_argument e("Error when load image: " + path + ":\n" +
                            SDL_GetError());
    throw e;
  }

  _mouseOutTexture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);
}

// Create the texture from the image path when the mouse hovers over the button
void ButtonTexture::loadMouseOverTexture(std::string path,
                                         SDL_Renderer *renderer) {
  SDL_Surface *image = IMG_Load(path.c_str());
  if (image == nullptr) {
    std::invalid_argument e("Error when load image: " + path + ":\n" +
                            SDL_GetError());
    throw e;
  }

  _mouseOverTexture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);
}
// Create the texture from the image path when the button is clicked
void ButtonTexture::loadMouseDownTexture(std::string path,
                                         SDL_Renderer *renderer) {
  SDL_Surface *image = IMG_Load(path.c_str());
  if (image == nullptr) {
    std::invalid_argument e("Error when load image: " + path + ":\n" +
                            SDL_GetError());
    throw e;
  }

  _mouseDownTexture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);
}

// Create the texture for the button text
void ButtonTexture::loadTextTexture(SDL_Surface *image,
                                    SDL_Renderer *renderer) {
  if (image == nullptr) {
    std::invalid_argument e("Something went wrong while loading the text.");
    throw e;
  }

  _textTexture = SDL_CreateTextureFromSurface(renderer, image);
}

// Display the button texture based on the button state
void ButtonTexture::render(Rect rect, SDL_Renderer *renderer,
                           ButtonState type) {
  SDL_Rect destination = {rect.position.x, rect.position.y, rect.size.width,
                          rect.size.height};
  float text_aspect_ratio = static_cast<float>(_textSize.width) /
                            static_cast<float>(_textSize.height);
  int text_height = destination.h * 0.8;
  int text_width = text_height * text_aspect_ratio;
  int text_y = destination.y + (destination.h - text_height) / 2;
  int text_x = destination.x + (destination.w - text_width) / 2;
  SDL_Rect text_destination = {text_x, text_y, text_width, text_height};
  switch (type) {
  case ButtonState::kMouseOut:
    SDL_RenderCopy(renderer, _mouseOutTexture, nullptr, &destination);
    SDL_RenderCopy(renderer, _textTexture, nullptr, &text_destination);
    break;
  case ButtonState::kMouseOver:
    SDL_RenderCopy(renderer, _mouseOverTexture, nullptr, &destination);
    SDL_RenderCopy(renderer, _textTexture, nullptr, &text_destination);
    break;
  case ButtonState::kMouseDown:
    SDL_RenderCopy(renderer, _mouseDownTexture, nullptr, &destination);
    SDL_RenderCopy(renderer, _textTexture, nullptr, &text_destination);
    break;
  default:
    break;
  }
}

int ButtonTexture::getWidth() { return _width; }

int ButtonTexture::getHeight() { return _height; }

void ButtonTexture::resize(const Size &size) {
  _width = size.width;
  _height = size.height;
}