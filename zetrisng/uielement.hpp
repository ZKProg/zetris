#pragma once

#include "pch.h"

class UIelement
{
public:
  explicit UIelement(const int& x, const int& y,
		     const int& w, const int& h,
		     SDL_Renderer *renderer,
		     const std::string& text, const int& fontSize = 16, 
		     const std::string& font = "./fonts/roboto-regular.ttf",
		     const SDL_Color& bgColor = {255, 255, 255, 255},
		     const SDL_Color& fontColor = {0, 0, 0, 255});
  virtual ~UIelement();

  void renderElement() const noexcept;
  
  bool isClicked(const int& mouseX, const int& mouseY) const;
  bool isHovered();
  
protected:

  int _zindex;
  int _x, _y, _w, _h;
  int _textX, _textY, _textW, _textH;
  std::string _font;
  std::string _id;

  SDL_Color _bgColor;
  SDL_Color _fontColor;
  TTF_Font *_ttfFont;
  SDL_Texture *_textTexture;
  SDL_Renderer *_renderer;
  
};
