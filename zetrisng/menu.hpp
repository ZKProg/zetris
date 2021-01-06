#pragma once

#include "pch.h"
#include "uielements.hpp"

class Menu
{
public:
  explicit Menu();
  virtual ~Menu();

protected:
  std::vector<UIelement> _UIelements;
};

