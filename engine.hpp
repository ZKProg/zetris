#pragma once

#include "pch.h"

#include "UIElement.hpp"
#include "grid.hpp"

using std::cout;
using std::endl;
using std::cerr;

class Engine
{
public:
  explicit Engine();
  virtual ~Engine();

  void initEngine();
  void mainLoop();
  void exitWithError(const std::string& error);

  void renderMenu();
  void renderGameScreen();
  void renderText(int x, int y,
		  const std::string& text,
		  const std::string& fontFile = ".fonts/Roboto-Thin.ttf");
protected:

  int _currentTetrominoX, _currentTetrominoY;
  SDL_Window *_mainWindow;
  SDL_Renderer *_mainRenderer;
  SDL_Event _events;

  bool _isRunning = true;
  bool _fpsToOSD = false;
 
  int _mouseX = 0;
  int _mouseY = 0;
  Uint32 _timeReference;
  Uint32 _fpsReference;

  float _numberOfFrames = 0;
  float _fps = 60.0f;
  
  SDL_Rect _bgRect = { 0, 0, 800, 800 };
  SDL_Color _bgColor = { 0, 0, 0, 255 };
  std::map<int, SDL_Texture*> _tetrominoTextures;
  
  std::vector<UIElement> _UIElements;
  std::unique_ptr<Grid> _grid;

  
  TTF_Font *_osdFont;
  
  enum GameState {
    HOME,
    PLAY,
    GAME_OVER,
    TRANSITION
  } _GameState;

};
