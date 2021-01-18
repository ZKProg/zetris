#include "pch.h"
#include "engine.hpp"
#include "globals.h"

int main (int argc, char **argv)
{
  GameMixin gameMixin(800, 600, "Zetris By ZKProg 0.1", "ZETRIS");

  
  Engine engine(gameMixin);
  engine.mainLoop();


  std::cout << "Zetris shutting down..." << std::endl;
  return 0;
}
