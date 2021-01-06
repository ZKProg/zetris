#pragma once

#include "pch.h"
#include "globals.h"
#include "menu.hpp"
#include "uielement.hpp"

class Engine
{
public:
  explicit Engine(GameMixin& gameMixin,
		  const std::string& osdFontFile = "./fonts/roboto-regular.ttf");
  virtual ~Engine();

  bool init();
  void mainLoop();
  bool exitWithError(const std::string& error);

  void renderUIs();
  void checkClickedUis();
  void renderGameSurface();
  void renderOSD(int x, int y);
  void renderText(int x, int y,
		  const std::string& text);
  void cleanWindowBackground();  

  // @TODO dev callbacks to remove
  static void sayBuy();
  
protected:
  GameMixin __gameMixin;

  SDL_Window *_mainWindow;
  SDL_Renderer *_mainRenderer;
  SDL_Event _event;

  bool _isRunning = true;
  bool _fpsToOSD = false;
  bool _isMouseButtonPressed = false;

  int _mouseX;
  int _mouseY;
  float _numberOfFrames;
  Uint32 _timeReference;
  Uint32 _fpsReference;

  SDL_Rect _bgRect;
  SDL_Color _bgColor;

  TTF_Font *_osdFont;
  GameState _gameState;
  std::string _osdFontFile;
  Menu _homeMenu;
};

