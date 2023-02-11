#ifndef RENDERER_H
#define RENDERER_H


#include <vector>
#include <memory>

#include "SDL.h"

#include "snake.h"
#include "dynamic_element.h"
#include "typedefs.h"

class Renderer {
public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(GameState const &game_state);
  void UpdateWindowTitle(int score, int fps);

private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  std::unique_ptr<DynamicElement> buttonBackground;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif