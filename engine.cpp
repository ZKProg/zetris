#include "engine.hpp"

Engine::Engine() :
  _mainWindow(nullptr), _mainRenderer(nullptr)
{
  _GameState = HOME;
  _timeReference = SDL_GetTicks();

  _currentTetrominoX = 0;
  _currentTetrominoY = 0;

}

Engine::~Engine()
{
  for (const auto& texture: _tetrominoTextures) {
    SDL_DestroyTexture(texture.second);
  }
  
  SDL_DestroyRenderer(_mainRenderer);
  IMG_Quit();
  TTF_Quit();
  SDL_DestroyWindow(_mainWindow);
  SDL_Quit();
}

void Engine::initEngine()
{
  // Main initialization
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    this->exitWithError(SDL_GetError());
  }

  // Main Window creation
  _mainWindow = SDL_CreateWindow("ZETRIS",
				 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				 800, 800,
				 SDL_WINDOW_SHOWN);

  if (_mainWindow == nullptr) {
    this->exitWithError(SDL_GetError());
  }

  // Main Renderer creatino
  _mainRenderer = SDL_CreateRenderer(_mainWindow,
				     -1,
				     SDL_RENDERER_ACCELERATED);

  if (_mainRenderer == nullptr) {
    this->exitWithError(SDL_GetError());
  }

  SDL_RenderClear(_mainRenderer);
      
  // Init SDL2 image library
  int flags = IMG_INIT_JPG | IMG_INIT_PNG;

  if ((IMG_Init(flags) & flags) != flags) {
    this->exitWithError(IMG_GetError());
  }

  // Font library init
  if (TTF_Init() < 0) {
    this->exitWithError(TTF_GetError());
  }

  // default osd font
  _osdFont = TTF_OpenFont("./fonts/Roboto-Thin.ttf", 16);
  if (_osdFont == nullptr) {
    std::cout << "The default OSD font could not be loaded." << std::endl;
    std::cout << TTF_GetError() << std::endl;
  }

  // Initialize the background
  // @TODO refactor that in the UI function/class
  // Creates the menu
  UIElement playElement("play",
			 "Play!",
			 _mainRenderer,
			 30, 30,
			 300, 40,
			 (SDL_Color){100, 100, 100, 255});

  UIElement quitElement("quit",
			"Quit Game",
			_mainRenderer,
			30, 150,
			300, 40,
			(SDL_Color){255, 0, 0, 255});

  UIElement scoreElement("scores",
			"Scores",
			_mainRenderer,
			30, 90,
			300, 40,
			(SDL_Color){0, 0, 255, 255});

  _UIElements.push_back(playElement);
  _UIElements.push_back(quitElement);
  _UIElements.push_back(scoreElement);

  _grid = std::unique_ptr<Grid>(new Grid(_mainRenderer));

  // Creates the tetromino textures and stores them in a container
  SDL_Surface *blocksSurface = IMG_Load("./textures/blocks.png");
  SDL_Texture *blocksTexture = SDL_CreateTextureFromSurface(_mainRenderer, blocksSurface);
  this->_tetrominoTextures.insert(std::make_pair(0, blocksTexture));
  SDL_FreeSurface(blocksSurface);
  
  // First rendering pass
  this->renderMenu();
}

void Engine::mainLoop()
{
  Uint32 mainLoopStartTime = SDL_GetTicks();
  _fpsReference = SDL_GetTicks();
  
  while (_isRunning) {
    
    while (SDL_PollEvent(&_events) > 0) {
      // Windows events -----------------------------------------
      if (_events.type == SDL_QUIT) _isRunning = false;


      // Mouse events -------------------------------------------
      if (_events.type == SDL_MOUSEBUTTONDOWN) {
	SDL_GetMouseState(&_mouseX, &_mouseY);

	// Test if a UIElement has been clicked
	// Only test if home menu
	if (_GameState == HOME) {
	  for (auto& element: _UIElements) {
	    if (element.isClicked(_mouseX, _mouseY)) {
	      // Test the UI clicked and react accordingly
	      if (element.getName() == "quit") {
		_isRunning = false;
		break;
	      }
	      if (element.getName() == "play") {
		_GameState = PLAY;
	      }
	    }
	  }
	}
      }

      // Keyboard events ----------------------------------------
      if (_events.type == SDL_KEYDOWN) {
	if (_events.key.keysym.sym == SDLK_ESCAPE) {
	  if (_GameState == HOME) _isRunning = false;
	  if (_GameState == PLAY) _GameState = HOME;
	}
	if (_events.key.keysym.sym == SDLK_F1) _fpsToOSD = !_fpsToOSD;
	if (_events.key.keysym.sym == SDLK_LEFT) {
	  _grid->moveLeft(_grid->_T4, _currentTetrominoX, _currentTetrominoY, 4);
	}
	if (_events.key.keysym.sym == SDLK_RIGHT) {
	  _grid->moveRight(_grid->_T4, _currentTetrominoX, _currentTetrominoY, 4);
	}
	if (_events.key.keysym.sym == SDLK_DOWN) {
	  _currentTetrominoY++;
	}

	
      }
	
    } // End while SDL_PollEvent ////////////////////////////////////////

    // Will render at a given - maximum - FPS, if able.
    if (SDL_GetTicks() - mainLoopStartTime > 1.f/100.f * 1000) {

      _numberOfFrames++;
      
      // Reset the timing condition
      mainLoopStartTime = SDL_GetTicks();
      
      // Rendering (condition based)
      if (_GameState == HOME) {
	// The game is running at the menu
	// Hence, render the Home UI
	this->renderMenu();
      
      }
      else if (_GameState == PLAY) {
	// The game begins, starts the tetris logic + rendering
	this->renderGameScreen();
      }
    }
    
  } // End while _isRunning //////////////////////////////////////////////////////////
  
}

void Engine::exitWithError(const std::string& error)
{
  cerr << error << endl;
  exit(-1);
}

void Engine::renderMenu()
{
  SDL_RenderClear(_mainRenderer);
  SDL_SetRenderDrawColor(_mainRenderer,
			 _bgColor.r,
			 _bgColor.g,
			 _bgColor.b,
			 _bgColor.a);
  SDL_RenderFillRect(_mainRenderer, &_bgRect);
  
  for (auto& element: _UIElements) {
    element.renderElement(_mainRenderer);
  }
  
  SDL_RenderPresent(_mainRenderer);

}

// Render the IN-GAME screen /////////////////////////////////////////////////////////
void Engine::renderGameScreen()
{
  // Render background
  SDL_SetRenderDrawColor(_mainRenderer,
			 _bgColor.r,
			 _bgColor.g,
			 _bgColor.b,
			 _bgColor.a);
  SDL_RenderFillRect(_mainRenderer, &_bgRect);

  // render the grid
  _grid->render(_mainRenderer, _tetrominoTextures[0]);

  // render the tetrominos
  // @TODO review: decides if this is the best way

  if (SDL_GetTicks() - _timeReference > 1000) {
    _timeReference = SDL_GetTicks();
    _currentTetrominoY++;
  }
  
  _grid->renderTetrominoAfterTest(_grid->_T4,
			 _mainRenderer,
			 _tetrominoTextures.at(0),
			 _currentTetrominoX,
			 _currentTetrominoY,
			 4);

  // OSD ouput
  if (_fpsToOSD) {
    float elapsedMs = SDL_GetTicks() - _fpsReference;
    float fps = 1000  / elapsedMs;
    _fpsReference = SDL_GetTicks();
  
    std::string fpsString = "Frame per second: " + std::to_string(fps) + "fps";
    this->renderText(500, 500, fpsString);
  }
  // Output render
  SDL_RenderPresent(_mainRenderer);

}


void Engine::renderText(int x, int y,
			const std::string& text,
			const std::string& fontFile)
{
  SDL_Surface *textSurface = TTF_RenderText_Solid(_osdFont,
						  text.c_str(),
						  {255, 255, 255, 255});

  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(_mainRenderer, textSurface);
  int w, h;
  SDL_QueryTexture(textTexture, nullptr, nullptr, &w, &h);
  SDL_Rect dst = { 790 - w, 800 - 30, w, h };
  SDL_RenderCopy(_mainRenderer,
		 textTexture,
		 nullptr,
		 &dst);

  SDL_FreeSurface(textSurface);
}
