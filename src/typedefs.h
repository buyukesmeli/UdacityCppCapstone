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
};

enum GameStatePhase {
  kMenu,
  kPlaying,
  kGameOver
};

enum FoodType {
  kNormal,
  kSpecial
};

struct Food{
  SDL_Point position;
  FoodType type;
};

struct GameState {
  Snake snake;
  Food food;
  // int score;
  GameStatePhase phase;
};


#endif