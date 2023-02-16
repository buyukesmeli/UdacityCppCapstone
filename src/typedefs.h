#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "snake.h"

struct Point {
  int x;
  int y;
};

struct Size {
  int width;
  int height;
};

struct Rect {
  Point position;
  Size size;
  bool contains(Point point) const {
    return point.x >= position.x && point.x <= position.x + size.width &&
           point.y >= position.y && point.y <= position.y + size.height;
  }
};

enum GameStatePhase { kMenu, kPlaying, kGameOver };

enum FoodType { kNormal, kSpecial };

enum ButtonState {
  kMouseOut,
  kMouseOver,
  kMouseDown,
};

struct Food {
  SDL_Point position;
  FoodType type;
};

struct GameState {
  Snake snake;
  Food food;
  Food specialFood;
  bool bSpecialFoodActive;
  bool bToggle;
  GameStatePhase phase;
};

#endif