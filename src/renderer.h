#pragma once
#include <memory>
#include <vector>

#include "SDL.h"

#include "button.h"
#include "dynamic_element.h"
#include "snake.h"
#include "typedefs.h"

class Renderer {
public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(std::vector<std::unique_ptr<DynamicElement>> const *UIElements);
  void Render(GameState const &game_state);

  void UpdateWindowTitle(int score, int fps);
  SDL_Renderer *getRenderer();
  std::size_t getScreenWidth() { return screen_width; }
  std::size_t getScreenHeight() { return screen_height; }
  std::size_t getGridWidth() { return grid_width; }
  std::size_t getGridHeight() { return grid_height; }

private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;

  void drawPlaying(const Snake &snake, const SDL_Point &food,
                   const bool &bSpecialFood, const SDL_Point &specialFood);
  void drawGameOver();
};
