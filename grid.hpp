#pragma once

#include "pch.h"

typedef unsigned short tetromino;

class Grid
{
public:
  explicit Grid(SDL_Renderer *renderer, int w = 10, int h = 20);
  virtual ~Grid();

  tetromino _T1[16] = {
    1, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
  };

  tetromino _T2[16] = {
    2, 2, 0, 0,
    2, 2, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
  };

  tetromino _T3[16] = {
    3, 3, 3, 0,
    0, 3, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
  };
  
  tetromino _T4[16] = {
    0, 4, 0, 0,
    0, 4, 0, 0,
    4, 4, 0, 0,
    0, 0, 0, 0
  };

  
  // functional tetromino logic
  void renderTetrominoAfterTest (tetromino *t, SDL_Renderer *renderer,
			SDL_Texture *texture,
			int& x, int& y, int type);

  
  void render(SDL_Renderer *renderer, SDL_Texture *tetrominoesTexture);
  std::vector<int> getGridSpaceCoordinates(int xTetromino, int yTetromino,
					   int xTetrominoSpace, int yTetrominoSpace);

  int getIndexFromCoordinates(int& x, int& y, int& w);
  bool canTetrominoMoveDownwards(tetromino *t, int& x, int& y, const int& yMax = 4);
  void persistTetrominoIntoGrid(tetromino *t, int& x, int& y, int& type, const int& yMax = 4);

  void moveRight(tetromino *t, int& x, int&y, int type);
  void moveLeft(tetromino *t, int& x, int&y, int type);
  
protected:

  int _w, _h;
  int _posX, _posY;
  std::unique_ptr<int> _grid;
  SDL_Texture *_borderTexture;

};


