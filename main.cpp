#include <iostream>

#include "engine.hpp"

using std::cout;
using std::endl;

int main (int argc, char **argv)
{
  cout << "Zetris running..." << endl;

  Engine engine;
  engine.initEngine();

  engine.mainLoop();
  
  return 0;
}
