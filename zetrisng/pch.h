#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <algorithm>
#include <map>
#include <thread>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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
  float _fps = 60.0f;
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
  
