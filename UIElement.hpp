#include "pch.h"

class UIElement
{
public:
  explicit UIElement(const std::string& name,
		     const std::string& text,
		     SDL_Renderer *renderer,
		     const int& posX, const int& posY,
		     const int& w, const int& h,
		     const SDL_Color& bgColor = {0, 0, 0, 255},
		     const std::string& font = "./fonts/stocky.ttf");
  virtual ~UIElement();

  bool isClicked(const int& mouseX, const int& mouseY) noexcept;
  void renderElement(SDL_Renderer  *renderer) noexcept;
  std::string getName() const;
  
protected:
  int _posX, _posY, _w, _h;
  SDL_Color _bgColor;
  SDL_Rect _UIRect;
  std::string _name;
  TTF_Font *_font;
  SDL_Texture *_fontTexture;
};
