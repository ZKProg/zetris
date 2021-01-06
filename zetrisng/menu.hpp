#pragma once

#include "pch.h"
#include "uielement.hpp"

class Menu
{
public:
  explicit Menu();
  virtual ~Menu();

  void addElement(const UIelement& element);
  void render();
  void checkMouseClick(const int& mouseX, const int& mouseY);
  
protected:
  std::vector<UIelement> _UIelements;
};

