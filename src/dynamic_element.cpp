#include "dynamic_element.h"
#include <SDL2/SDL_image.h>

#include <stdexcept>

// Define basic UI element properties

void DynamicElement::move(const Point &position) { _position = position; }

void DynamicElement::resize(const Size &size) { _size = size; }

Rect DynamicElement::getRect() const { return {_position, _size}; }

Point DynamicElement::getPosition() const { return _position; }

Size DynamicElement::getSize() const { return _size; }
