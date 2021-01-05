#pragma once

#include "pch.h"

// Custom defined data/types
struct GameMixin
{
  GameMixin(int w, int h, std::string winTitle, std::string gameName)
  {
    _winWidth = w;
    _winHeight = h;
    _winTitle = winTitle;
    _gameName = gameName;
  }
  
  int _winWidth = 800;
  int _winHeight = 600;
  int _osdFontSize = 16;
  std::string _winTitle = "";
  std::string _gameName = "";
  float _targetFps = 600000.0f;
  unsigned short _numThreads = 4;
  bool _fpsToOSD = false;
  SDL_Color _backgroundColor = {0, 0, 0, 255}; // Back background color
};

enum GameState {
  HOME_MENU,
  IN_GAME,
  GAME_OVER,
  LOADING
};

// Exception definition(s)
class EngineException : public std::exception
{
 public:
  virtual const char* what() const throw()
  {
    return "Failure to initialize the engine.";
  }
} static engineException;
