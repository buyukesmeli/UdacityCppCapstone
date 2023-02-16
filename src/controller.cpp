#include "controller.h"
#include "SDL.h"
#include "snake.h"
#include <iostream>

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1)
    snake.direction = input;
  return;
}

// Process input from the user for the dyamic element list
void Controller::HandleInput(
    bool &running,
    std::vector<std::unique_ptr<DynamicElement>> *UIElements) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    }
    for (auto &element : *UIElements) {
      element->handleEvent(&e);
    }
  }
}

// Process input from the user for the game
void Controller::HandleInput(bool &running, GameState &gameState) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
      case SDLK_UP:
        ChangeDirection(gameState.snake, Snake::Direction::kUp,
                        Snake::Direction::kDown);
        break;

      case SDLK_DOWN:
        ChangeDirection(gameState.snake, Snake::Direction::kDown,
                        Snake::Direction::kUp);
        break;

      case SDLK_LEFT:
        ChangeDirection(gameState.snake, Snake::Direction::kLeft,
                        Snake::Direction::kRight);
        break;

      case SDLK_RIGHT:
        ChangeDirection(gameState.snake, Snake::Direction::kRight,
                        Snake::Direction::kLeft);
        break;
      }
    }
  }
}