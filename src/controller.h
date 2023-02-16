#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "dynamic_element.h"
#include "snake.h"
#include "typedefs.h"
#include <memory>

class Controller {
public:
  void HandleInput(bool &running, GameState &gameState) const;
  void
  HandleInput(bool &running,
              std::vector<std::unique_ptr<DynamicElement>> *UIElements) const;

private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif