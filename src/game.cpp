#include <iostream>
#include <thread>

#include "SDL.h"
#include "button.h"
#include "game.h"
#include "text_label.h"
#include "typedefs.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : engine(dev()), _gridWidth(static_cast<int>(grid_width)),
      _gridHeight(static_cast<int>(grid_height)),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {

  _gameState = std::make_unique<GameState>();
  _gameState->phase = GameStatePhase::kMenu;
  ResetGame(_gameState);


  // Start the thread for flashing the special food
  _thread = std::thread([this]() {
    bool toggle = true;
    while (toggle) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      {
        std::lock_guard<std::mutex> lg(_mutex);
        this->_toggle_var = !(this->_toggle_var);
        toggle = this->_bToggle;
      }
    }
  });
}
Game::~Game() { _thread.join(); }

void Game::setRenderer(Renderer *renderer) { _renderer = renderer; }

void Game::setController(Controller *controller) { _controller = controller; }


// Create all the UI elements for menu and game over screens
void Game::setUI() {
  // Create UI elements:
  auto screen_width = _renderer->getScreenWidth();
  auto screen_height = _renderer->getScreenHeight();
  SDL_Color buttonForegroundColor = {50, 50, 50, 0};

  // New Game Screen
  // 1. New Game Button
  Point newGameButtonPosition = {static_cast<int>(screen_width / 3),
                                 static_cast<int>(3 * screen_height / 10)};
  Size newGameButtonSize = {static_cast<int>(screen_width / 3),
                            static_cast<int>(screen_width / 14)};
  std::unique_ptr<Button> newGameButton = std::make_unique<Button>();
  newGameButton->setRenderer(_renderer->getRenderer());
  newGameButton->move(newGameButtonPosition);
  newGameButton->resize(newGameButtonSize);
  newGameButton->setText("New Game");
  newGameButton->setFont("../assets/font/Roboto-Regular.ttf",
                         buttonForegroundColor, 256);
  newGameButton->setMouseDownTexture("../assets/images/button_down.png");
  newGameButton->setMouseOverTexture("../assets/images/button_over.png");
  newGameButton->setMouseOutTexture("../assets/images/button_out.png");
  newGameButton->setMouseUpEvent(&Game::newGameButtonUpEvent, this);

  _menu.push_back(std::move(newGameButton));

  // 2. Options Button
  Point optionsButtonPosition = {static_cast<int>(screen_width / 3),
                                 static_cast<int>(4 * screen_height / 10)};
  Size optionsButtonSize = {static_cast<int>(screen_width / 3),
                            static_cast<int>(screen_width / 14)};
  std::unique_ptr<Button> optionsButton = std::make_unique<Button>();
  optionsButton->setRenderer(_renderer->getRenderer());
  optionsButton->move(optionsButtonPosition);
  optionsButton->resize(optionsButtonSize);
  optionsButton->setText("Options");
  optionsButton->setFont("../assets/font/Roboto-Regular.ttf",
                         buttonForegroundColor, 256);
  optionsButton->setMouseDownTexture("../assets/images/button_down.png");
  optionsButton->setMouseOverTexture("../assets/images/button_over.png");
  optionsButton->setMouseOutTexture("../assets/images/button_out.png");

  _menu.push_back(std::move(optionsButton));

  // 3. Quit Button
  Point quitButtonPosition = {static_cast<int>(screen_width / 3),
                              static_cast<int>(5 * screen_height / 10)};
  Size quitButtonSize = {static_cast<int>(screen_width / 3),
                         static_cast<int>(screen_width / 14)};
  std::unique_ptr<Button> quitButton = std::make_unique<Button>();
  quitButton->setRenderer(_renderer->getRenderer());
  quitButton->move(quitButtonPosition);
  quitButton->resize(quitButtonSize);
  quitButton->setText("Quit");
  quitButton->setFont("../assets/font/Roboto-Regular.ttf",
                      buttonForegroundColor, 256);
  quitButton->setMouseDownTexture("../assets/images/button_down.png");
  quitButton->setMouseOverTexture("../assets/images/button_over.png");
  quitButton->setMouseOutTexture("../assets/images/button_out.png");
  quitButton->setMouseUpEvent(&Game::quitButtonUpEvent, this);

  _menu.push_back(std::move(quitButton));

  // Game Over Screen
  // 1. Score Text
  Point scoreTextPosition = {static_cast<int>(screen_width / 3),
                             static_cast<int>(3 * screen_height / 10)};
  Size scoreTextSize = {static_cast<int>(screen_width / 3),
                        static_cast<int>(screen_width / 14)};
  SDL_Color scoreTextColor = {250, 250, 250, 0};
  std::unique_ptr<TextLabel> scoreText = std::make_unique<TextLabel>();
  scoreText->setRenderer(_renderer->getRenderer());
  scoreText->move(scoreTextPosition);
  scoreText->resize(scoreTextSize);
  std::string scoreTextString = "Score: " + std::to_string(score);
  scoreText->setText(scoreTextString);
  scoreText->setFont("../assets/font/Roboto-Regular.ttf", 256, scoreTextColor);

  _gameover.push_back(std::move(scoreText));

  // 2. New Game Button
  Point newGameButtonPosition2 = {static_cast<int>(screen_width / 3),
                                  static_cast<int>(6 * screen_height / 10)};
  Size newGameButtonSize2 = {static_cast<int>(screen_width / 3),
                             static_cast<int>(screen_width / 14)};
  std::unique_ptr<Button> newGameButton2 = std::make_unique<Button>();
  newGameButton2->setRenderer(_renderer->getRenderer());
  newGameButton2->move(newGameButtonPosition2);
  newGameButton2->resize(newGameButtonSize2);
  newGameButton2->setText("New Game");
  newGameButton2->setFont("../assets/font/Roboto-Regular.ttf",
                          buttonForegroundColor, 256);
  newGameButton2->setMouseDownTexture("../assets/images/button_down.png");
  newGameButton2->setMouseOverTexture("../assets/images/button_over.png");
  newGameButton2->setMouseOutTexture("../assets/images/button_out.png");
  newGameButton2->setMouseUpEvent(&Game::newGameButtonUpEvent, this);

  _gameover.push_back(std::move(newGameButton2));
}

// Define button event handlers
void Game::newGameButtonUpEvent() {
  _gameState->phase = GameStatePhase::kPlaying;
  ResetGame(_gameState);
}

void Game::quitButtonUpEvent() {
  std::lock_guard<std::mutex> lock(_mutex);
  _bToggle = false;
  running = false;
}

void Game::Run(std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;

  // Calculate the number of frames to render for a
  // special food item
  specialFoodTime = 5000 / target_frame_duration;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.

    // Call different control handlers for different screens
    if (_gameState->phase == GameStatePhase::kMenu)
      _controller->HandleInput(running, &_menu);
    else if (_gameState->phase == GameStatePhase::kPlaying)
      _controller->HandleInput(running, *_gameState);
    else if (_gameState->phase == GameStatePhase::kGameOver)
      _controller->HandleInput(running, &_gameover);

    Update(*_gameState);

    {
      std::lock_guard<std::mutex> lock(_mutex);
      _gameState->bToggle = _toggle_var;
    }

    // Call different render handlers for different screens
    if (_gameState->phase == GameStatePhase::kMenu)
      _renderer->Render(&_menu);
    else if (_gameState->phase == GameStatePhase::kPlaying)
      _renderer->Render(*_gameState);
    else if (_gameState->phase == GameStatePhase::kGameOver) {
      _gameover[0]->setText("Score: " + std::to_string(score));
      _renderer->Render(&_gameover);
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loPop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      _renderer->UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  std::lock_guard<std::mutex> lock(_mutex);
  _bToggle = false;
}

void Game::PlaceFood(GameState &gameState) {
  if (!gameState.snake.alive) {
    return;
  }
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    if (gameState.bSpecialFoodActive) {
      // Check that the location is not occupied by a special food item before
      // placing food.
      if (x == gameState.specialFood.position.x &&
          y == gameState.specialFood.position.y) {
        continue;
      }
    }
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!gameState.snake.SnakeCell(x, y)) {
      gameState.food.position.x = x;
      gameState.food.position.y = y;
      return;
    }
  }
}
void Game::PlaceSpecialFood(GameState &gameState) {
  if (!gameState.snake.alive) {
    return;
  }
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item or food item before
    // placing special food.
    if (!gameState.snake.SnakeCell(x, y)) {
      if (gameState.food.position.x == x && gameState.food.position.y == y) {
        continue;
      }
      gameState.specialFood.position.x = x;
      gameState.specialFood.position.y = y;
      return;
    }
  }
}

void Game::Update(GameState &gameState) {
  if (gameState.phase == GameStatePhase::kPlaying && !gameState.snake.alive) {
    gameState.phase = GameStatePhase::kGameOver;
    return;
  }

  if (gameState.phase == GameStatePhase::kMenu ||
      gameState.phase == GameStatePhase::kGameOver) {
    return;
  }

  gameState.snake.Update();

  int new_x = static_cast<int>(gameState.snake.head_x);
  int new_y = static_cast<int>(gameState.snake.head_y);

  // Update Special Food
  if (gameState.bSpecialFoodActive) {
    _specialFoodTimer++;
    if (gameState.specialFood.position.x == new_x &&
        gameState.specialFood.position.y == new_y) {
      score += 10;
      gameState.bSpecialFoodActive = false;
      gameState.specialFood.position.x = 0;
      gameState.specialFood.position.y = 0;
      gameState.snake.speed += 0.02;
    } else if (_specialFoodTimer >= specialFoodTime) {
      gameState.bSpecialFoodActive = false;
      gameState.specialFood.position.x = 0;
      gameState.specialFood.position.y = 0;
    }
  }

  // Check if there's food over here
  if (gameState.food.position.x == new_x &&
      gameState.food.position.y == new_y) {
    score++;
    _specialFoodCounter++;
    PlaceFood(gameState);
    // Grow snake and increase speed.
    gameState.snake.GrowBody();
    gameState.snake.speed += 0.02;

    // Check if special food should be placed
    if (_specialFoodCounter >= 5) {
      gameState.bSpecialFoodActive = true;
      _specialFoodTimer = 0;
      PlaceSpecialFood(gameState);
      _specialFoodCounter = 0;
    }
  }
}

void Game::ResetGame(std::unique_ptr<GameState> &gameState) {
  gameState->snake.Reset();
  gameState->food.type = FoodType::kNormal;
  gameState->food.position.x = 0;
  gameState->food.position.y = 0;
  gameState->specialFood.position.x = 0;
  gameState->specialFood.position.y = 0;
  gameState->bSpecialFoodActive = false;
  gameState->snake.alive = true;
  gameState->snake.SetGridSize(_gridWidth, _gridHeight);
  score = 0;
  _specialFoodCounter = 0;
  _specialFoodTimer = 0;
  PlaceFood(*gameState);
}
int Game::GetScore() const { return score; }
int Game::GetSize() const { return _gameState->snake.size; }