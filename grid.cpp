#include "grid.hpp"

// @TODO create a struct to represent 2D coordinates to preplace gsCoords use

Grid::Grid(SDL_Renderer *renderer, int w, int h)
  : _w(w), _h(h), _borderTexture(nullptr),
    _posX(20), _posY(100)
{

  _grid = std::unique_ptr<int>(new int[_w * _h]);
  
  SDL_Surface *_borderSurface = IMG_Load("./textures/borders.png");
  if (_borderSurface == nullptr) {
    // texture could not be opened
  }
  else {
    _borderTexture = SDL_CreateTextureFromSurface(renderer, _borderSurface);
    SDL_FreeSurface(_borderSurface);
  }

  // Initialize the grid (filled with zeros)
  for (int i = 0; i < _h; i++) {
    for (int j = 0; j < _w; j++) {
      // 10 represents the empty value in the grid (no tetromino)
      _grid.get()[i * _w + j] = 10;
    }
  }
  
}


Grid::~Grid()
{


}


void Grid::render(SDL_Renderer *renderer, SDL_Texture *tetrominoesTexture)
{
  SDL_Rect src = {0, 0, 32, 32};
  
  // Left border
  for (int i = 0; i < _h + 1; i++) {
    SDL_Rect dst = { _posX, _posY + i * 32, 32, 32 };
    SDL_RenderCopy(renderer, _borderTexture, &src, &dst); 
  }

  int rightBorderOffset = _posX + _w  * 32 + 32;
  
  // Right border
  for (int i = 0; i < _h + 1; i++) {
    SDL_Rect dst = { rightBorderOffset, _posY + i * 32, 32, 32 };
    SDL_RenderCopy(renderer, _borderTexture, &src, &dst); 
  }

  // Bottom border
  for (int i = 0; i < _w + 1; i++) {
    SDL_Rect dst = { _posX + i * 32, _posY + _h * 32, 32, 32 };
    SDL_RenderCopy(renderer, _borderTexture, &src, &dst); 
  }

  // Render the already persisted tetrominoes
  for (int i = 0; i < _h; i++) {
    for (int j = 0; j < _w; j++) {

      int type = _grid.get()[i * _w + j];

      if (type != 10) {

	int xOffset = type * 32;
	src = {xOffset, 0, 32, 32};
	SDL_Rect dst = { _posX + j * 32 + 32, _posY + i * 32, 32, 32 };
	SDL_RenderCopy(renderer, tetrominoesTexture, &src, &dst);

      }
      else {
	// No tetromino is present at this place in the grid.
      }
      
    }
  }
}


void Grid::renderTetrominoAfterTest (tetromino *t, SDL_Renderer *renderer,
				     SDL_Texture *texture,
				     int& x, int& y, int type) 
{
  // Test if the move is legal
  int minX = 3;
  int maxX = 0;
  int minY = 3;
  int maxY = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (t[i * 4 + j] != 0) {

	// detect the lateral min and max column
	// that is filled
	if (j > maxX) maxX = j;
	if (j < minX) minX = j;
	if (i > maxY) maxY = i;
	if (i < minY) minY = i;

      }
    }
  }

  // Bound movements laterally (against lateral borders)
  if (x + minX < 0) x = 0;
  if (x + maxX > _w - 1) x--;
  // Test if moves are legal, and persists if necessary
  if (!canTetrominoMoveDownwards(t, x, y, minY)) {
    persistTetrominoIntoGrid(t, x, y, type, minY);
  }

  // Test if the move reached the bottom 

  // Render the current tetromino
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {

      // @TODO simplify! Like in the grid rendering
      SDL_Rect src;
      src = { type * 32, 0, 32, 32};
	
      SDL_Rect dst = {_posX + x * 32 + 32 + j * 32, _posY + y * 32 + i * 32, 32, 32 };
      if (t[i * 4 + j] != 0) SDL_RenderCopy(renderer, texture, &src, &dst);
      
    }
  }
}

std::vector<int> Grid::getGridSpaceCoordinates(int xTetromino, int yTetromino,
					       int xTetrominoSpace, int yTetrominoSpace)
{

  int xGridSpace = xTetromino + xTetrominoSpace;
  int yGridSpace = yTetromino + yTetrominoSpace;

  return std::vector<int>({xGridSpace, yGridSpace});
  
}

int Grid::getIndexFromCoordinates(int& x, int& y, int& w) { return y * w + x; }

bool Grid::canTetrominoMoveDownwards(tetromino *t, int& x, int& y, const int& yMax)
{

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {

      // Get current block grid space coordinates
      std::vector<int> gsCoords = getGridSpaceCoordinates(j, i, x, y);
      std::vector<int> gsCoordsBelow = getGridSpaceCoordinates(j, i + 1, x, y);
   
      int index = getIndexFromCoordinates(gsCoords[0], gsCoords[1], _w);
      int indexBelow = getIndexFromCoordinates(gsCoordsBelow[0], gsCoordsBelow[1], _w);
      
      // Check if block is free or occupied
      if (t[i * 4 + j] != 0 && _grid.get()[indexBelow] != 10) {
	return false;
      }
      // Check if the tetromino reached the bottom
      if (y + yMax > _h) {
	return false;
      }
    }
  }
  
  return true;
}


void Grid::persistTetrominoIntoGrid(tetromino *t, int& x, int& y, int& type, const int& yMax)
{

  std::cout << "PERSIST!" << std::endl;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {

      // Get current block grid space coordinates
      std::vector<int> gsCoords = getGridSpaceCoordinates(j, i, x, y);
      int index = getIndexFromCoordinates(gsCoords[0], gsCoords[1], _w);

      //std::cout << index << "=>" << type << "=>" << i * 4 + j << std::endl;
      if (t[i * 4 + j] != 0) _grid.get()[index] = type;

    }
  }

  // reset
  x = 0; y = 0;
    
  
}

void Grid::moveRight(tetromino *t, int& x, int& y, int type)
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {

      // Get current block grid space coordinates
      std::vector<int> gsCoords = getGridSpaceCoordinates(j, i, x, y);
      std::vector<int> gsCoordsRight = getGridSpaceCoordinates(j + 1, i, x, y);
   
      int index = getIndexFromCoordinates(gsCoords[0], gsCoords[1], _w);
      int indexLeft = getIndexFromCoordinates(gsCoordsRight[0], gsCoordsRight[1], _w);
      
      // Check if block is free or occupied
      if (t[i * 4 + j] != 0 && _grid.get()[indexLeft] != 10) {
	return;
      }

    }
  }

  // Passed the test: can move right.
  x++;
}

void Grid::moveLeft(tetromino *t, int& x, int& y, int type)
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {

      // Get current block grid space coordinates
      std::vector<int> gsCoords = getGridSpaceCoordinates(j, i, x, y);
      std::vector<int> gsCoordsLeft = getGridSpaceCoordinates(j - 1, i, x, y);
   
      int index = getIndexFromCoordinates(gsCoords[0], gsCoords[1], _w);
      int indexLeft = getIndexFromCoordinates(gsCoordsLeft[0], gsCoordsLeft[1], _w);
      
      // Check if block is free or occupied
      if (t[i * 4 + j] != 0 && _grid.get()[indexLeft] != 10) {
	return;
      }

    }
  }

  // Passed the test: can move left.
  x--;

}
