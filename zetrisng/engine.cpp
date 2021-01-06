#include "engine.hpp"

Engine::Engine(GameMixin& gameMixin, const std::string& osdFontFile)
  : __gameMixin(gameMixin)
{
  _osdFontFile = osdFontFile;
  _bgRect = {0, 0, __gameMixin._winWidth, __gameMixin._winHeight};
  _gameState = HOME_MENU;

  try {
    if (this->init()) {
      std::cout << "Core engine successfully initialized." << std::endl;
    }
    else {
      throw engineException;
    }
  }
  catch(std::exception &e) {
    std::cout << "The game could not start: " << e.what() << std::endl;
  }
}


Engine::~Engine()
{
  // Leave and "clean" SDL libs/context
  SDL_DestroyRenderer(_mainRenderer);
  SDL_DestroyWindow(_mainWindow);
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
}


bool Engine::init()
{
  // SDL Init
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return exitWithError(SDL_GetError());

  // Main window creation
  _mainWindow = SDL_CreateWindow(__gameMixin._winTitle.c_str(),
  				 SDL_WINDOWPOS_CENTERED,
  				 SDL_WINDOWPOS_CENTERED,
  				 __gameMixin._winWidth,
  				 __gameMixin._winHeight,
				 SDL_WINDOW_SHOWN);
  
  
  if (_mainWindow == nullptr) exitWithError(SDL_GetError());

  // Main Renderer init
  _mainRenderer = SDL_CreateRenderer(_mainWindow, -1, SDL_RENDERER_ACCELERATED);
  if (_mainRenderer == nullptr) return exitWithError(SDL_GetError());

  // SDL2 image init
  int flags = IMG_INIT_JPG | IMG_INIT_PNG;
  if ((IMG_Init(flags) & flags) != flags) return exitWithError(IMG_GetError());

  // SDL2 font init
  if (TTF_Init() < 0) return exitWithError(TTF_GetError());
  // Create the default osd font
  _osdFont = TTF_OpenFont(_osdFontFile.c_str(), __gameMixin._osdFontSize);
  if (_osdFont == nullptr) exitWithError(TTF_GetError());

  // UI ---------------------------------------------------------------------------
  // @TODO add real menu
  UIelement firstElement(30, 30, 300, 50, _mainRenderer, "PLAY");
  UIelement secondElement(30, 90, 300, 50, _mainRenderer, "About");
  UIelement quitElement(30, 150, 300, 50, _mainRenderer, "Quit");
  quitElement.setCallbackFunction(sayBuy);
  _homeMenu.addElement(firstElement);
  _homeMenu.addElement(secondElement);
  _homeMenu.addElement(quitElement);
  
  // Initialization is complete. Clean the screen before proceeding.
  cleanWindowBackground();
   
  return true;
}

///////////////////////////////////////////////////////////////////////////////////
// MAIN LOOP
void Engine::mainLoop()
{
  std::cout << "Game " << __gameMixin._gameName << " starting." << std::endl;
  std::cout << "Target FPS of " << __gameMixin._targetFps << "fps." << std::endl;
  
  // init the time reference for fps counting and fps based rendering
  _fpsReference = SDL_GetTicks();
  // for 60fps, the duration of a frame should be of 16.66ms
  float frameMs = 1.f / __gameMixin._targetFps * 1000; // x1000 as of milliseconds
  
  while (_isRunning) {

    ////////////////////////////////////////////////////////////////////////////////
    // EVENTS
    while (SDL_PollEvent(&_event) > 0) {

      // Window events ////////////////////////////////////////////////////////////
      if (_event.type == SDL_QUIT) _isRunning = false;

      // Keyboard events //////////////////////////////////////////////////////////
      if (_event.type == SDL_KEYDOWN) {
	int keysym = _event.key.keysym.sym;

	switch (keysym) {
	case SDLK_ESCAPE:
	  _isRunning = false;
	  break;
	case SDLK_F1:
	  _fpsToOSD = !_fpsToOSD;
	  break;
	
	}
      }

      // Mouse events /////////////////////////////////////////////////////////////
      if (_event.type == SDL_MOUSEBUTTONDOWN) {
	SDL_GetMouseState(&_mouseX, &_mouseY);
	_isMouseButtonPressed = true;

	this->checkClickedUis();
	
      }

      if (_event.type == SDL_MOUSEBUTTONUP) {
	SDL_GetMouseState(&_mouseX, &_mouseY);
	_isMouseButtonPressed = false;
      }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // ENGINE CALLS
    if (SDL_GetTicks() - _fpsReference > frameMs) {
      // can call the fps rendering/engine based methods,
      // and depending on modes (HOME, MENU...)
      this->cleanWindowBackground();
      this->renderUIs();

      
      // Rendering OSD if enabled
      _numberOfFrames = 1000.f / (SDL_GetTicks() - _fpsReference);
      if (_fpsToOSD) this->renderOSD(5, 5);

      // Render on screen the whole bunch
      SDL_RenderPresent(_mainRenderer);

      // reinitialize the timestamp reference for next frame 
      _fpsReference = SDL_GetTicks();

      SDL_RenderClear(_mainRenderer);
    }
      
    
  }
  
}

bool Engine::exitWithError(const std::string& error)
{
  std::cout << "Initialization error: " << error.c_str() << std::endl;
  return false;
}


void Engine::renderUIs()
{
  if (_gameState == HOME_MENU) {
    _homeMenu.render();
  }
  else if (_gameState == IN_GAME) {


  }
}

void Engine::checkClickedUis()
{
  if (_gameState == HOME_MENU) {
    _homeMenu.checkMouseClick(_mouseX, _mouseY);
  }

}

void Engine::renderGameSurface()
{

}

void Engine::renderOSD(int x, int y) {
  
  // @TODO To improve! Needs a lot more information to display.
  std::string fpsMessage = std::string("FPS: ") + std::to_string(_numberOfFrames);
  fpsMessage += " Target FPS: " + std::to_string(__gameMixin._targetFps);
  renderText(x, y, fpsMessage);


}


void Engine::renderText(int x, int y,
			const std::string& text)
{
  SDL_Surface *textSurface = TTF_RenderText_Solid(_osdFont, text.c_str(), {255, 255, 255, 255});
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(_mainRenderer, textSurface);

  int w, h;
  SDL_QueryTexture(textTexture, nullptr, nullptr, &w, &h);
  SDL_Rect dst = {x, y, w, h};
  SDL_RenderCopy(_mainRenderer, textTexture, nullptr, &dst);

  SDL_FreeSurface(textSurface);
}


void Engine::cleanWindowBackground()
{
  // @TODO see if commented out are kept
  //SDL_RenderClear(_mainRenderer);
  SDL_SetRenderDrawColor(_mainRenderer,
			 __gameMixin._backgroundColor.r,
			 __gameMixin._backgroundColor.g,
			 __gameMixin._backgroundColor.b,
			 __gameMixin._backgroundColor.a);
  SDL_RenderFillRect(_mainRenderer, &_bgRect);
  //SDL_RenderPresent(_mainRenderer);

}


// Callbacks //////////////////////////////////////////////////////////////////////////
void Engine::sayBuy()
{
  exit(0);
}
