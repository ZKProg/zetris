#include "uielement.hpp"

UIelement::UIelement(const int& x, const int& y,
		     const int& w, const int& h,
		     SDL_Renderer *renderer,
		     const std::string& text, const int& fontSize, 
		     const std::string& font,
		     const SDL_Color& bgColor,
		     const SDL_Color& fontColor)
  : _callbackFunction(nullptr), _x(x), _y(y), _w(w), _h(h), _font(font),
    _bgColor(bgColor), _fontColor(_bgColor), 
    _ttfFont(nullptr), _textTexture(nullptr), _renderer(renderer)
{
  _id = text.substr(0, 10);
  _ttfFont = TTF_OpenFont(font.c_str(), fontSize);

  if (_ttfFont == nullptr) {
    std::cout << "The font " << font << " could not be loaded." << std::endl;
  }

  SDL_Surface *textSurface = TTF_RenderText_Solid(_ttfFont, text.c_str(), fontColor);
  _textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);
  SDL_QueryTexture(_textTexture, nullptr, nullptr, &_textW, &_textH);

  _textX = (_w - _textW) / 2;
  _textY = (_h - _textH) / 2;
  
  SDL_FreeSurface(textSurface);
}


UIelement::~UIelement()
{
  if (_callbackFunction != nullptr) {
    // @TODO check: not sure the behavior is predictable here
    delete (int*)_callbackFunction;
  }
}

void UIelement::renderElement() const noexcept
{
  SDL_Rect rectBackground = {_x, _y, _w, _h};
  SDL_Rect rectText = {_x + _textX, _y + _textY, _textW, _textH};
  SDL_SetRenderDrawColor(_renderer,
			 _bgColor.r,
			 _bgColor.g,
			 _bgColor.b,
			 _bgColor.a);
  SDL_RenderFillRect(_renderer, &rectBackground);
  SDL_RenderCopy(_renderer, _textTexture, nullptr, &rectText);
}


bool UIelement::isClicked(const int& mouseX, const int& mouseY) const
{
  if ((mouseX > _x && mouseX < _x + _w)
      &&
      (mouseY > _y && mouseY < _y + _h)) {

    if (_callbackFunctionSet && _callbackFunction != nullptr) {
      _callbackFunction();
    }
    
    // @TODO remove the id text cout
    std::cout << _id << " clicked." << std::endl;
    return true;
  }
  

  return false;
}


bool UIelement::isHovered()
{
  // @TODO implement

  return false;
}


void UIelement::setCallbackFunction(void (*callback)())
{
  _callbackFunction = callback;
  _callbackFunctionSet = true;
}
