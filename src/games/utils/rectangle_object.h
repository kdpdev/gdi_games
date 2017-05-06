#pragma once;

#include <engine/game_object.h>

class RectangleObject : public Engine::GameObject
{
public:
  RectangleObject(const Size& size, const Color& color);

  virtual void Draw(const MemDC& dc) const override;

private:
  Color m_color;
};
