#include "menu.hpp"

Menu::Menu()
{


}


Menu::~Menu()
{

  
}


void Menu::addElement(const UIelement& element)
{
  _UIelements.push_back(element);
}


void Menu::render()
{
  for (const auto& element: _UIelements) {
    element.renderElement();
  }
}

