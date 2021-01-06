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

protected:
  std::vector<UIelement> _UIelements;
};

