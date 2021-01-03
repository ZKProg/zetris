#include "UIElement.hpp"

UIElement::UIElement(const std::string& name,
		     const std::string& text,
		     SDL_Renderer *renderer,
		     const int& posX, const int& posY,
		     const int& w, const int& h,
		     const SDL_Color& bgColor,
		     const std::string& font)
  : _posX(posX), _posY(posY), _w(w), _h(h),
    _font(nullptr), _fontTexture(nullptr)
{
  _UIRect = { posX, posY, w, h };
  _bgColor = bgColor;
  _name = name;

  _font = TTF_OpenFont(font.c_str(), 40);
  if (_font == nullptr) {
    std::cout << TTF_GetError() << std::endl;
  }

  SDL_Surface *fontSurface =
    TTF_RenderText_Solid(_font,
			 text.c_str(),
			 (SDL_Color){255, 255, 255, 255});

  _fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
}

UIElement::~UIElement()
{


}

bool UIElement::isClicked(const int &mouseX, const int &mouseY) noexcept
{
  if (mouseX < (_posX + _w) &&
      mouseX > _posX        &&
      mouseY < (_posY + _h) &&
      mouseY > _posY) {
    std::cout << _name << " clicked." << std::endl;
    return true;
  }

  return false;
}

void UIElement::renderElement(SDL_Renderer *renderer) noexcept
{
  SDL_SetRenderDrawColor(renderer,
			 _bgColor.r,
			 _bgColor.g,
			 _bgColor.b,
			 _bgColor.a);
  SDL_RenderFillRect(renderer, &_UIRect);

  SDL_Rect dest = {_posX, _posY, _w, _h};
  SDL_RenderCopy(renderer,
		 _fontTexture,
		 nullptr,
		 &dest);
		 
}

std::string UIElement::getName() const
{
  return this->_name;
}
