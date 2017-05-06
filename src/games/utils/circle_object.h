#pragma once;

#include <engine/game_object.h>

class CircleObject : public Engine::GameObject
{
public:
  CircleObject(Length radius, const Color& color);

  virtual void Draw(const MemDC& dc) const override;

protected:
  virtual bool HandleMouseClick(const Point& pos) override;
  virtual bool HandleMouseMove(const Point& pos) override;

private:
  Color m_color;
};
