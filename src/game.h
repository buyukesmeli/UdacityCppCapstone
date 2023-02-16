#ifndef GAME_H
#define GAME_H
#include <chrono>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

#include "SDL.h"

#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  void Run(std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  void setRenderer(Renderer *renderer);
  void setController(Controller *controller);
  void setUI();

private:
  std::vector<std::unique_ptr<DynamicElement>> _menu;
  std::vector<std::unique_ptr<DynamicElement>> _gameover;
  std::unique_ptr<GameState> _gameState;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::mutex _mutex;
  std::thread _thread;

  Renderer *_renderer;
  Controller *_controller;

  int score{0};
  bool _toggle_var = false;
  bool _bToggle = true;
  bool running = true;
  int _gridWidth;
  int _gridHeight;
  int _specialFoodCounter;
  int _specialFoodTimer;

  void PlaceFood(GameState &gameState);
  void PlaceSpecialFood(GameState &gameState);
  void Update(GameState &gameState);
  void ResetGame(std::unique_ptr<GameState> &gameState);

  void newGameButtonUpEvent();
  void quitButtonUpEvent();

  int specialFoodTime;
};

#endif