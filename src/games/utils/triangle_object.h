#pragma once

#include <engine/game_object.h>

class TriangleObject : public Engine::GameObject
{
public:
  TriangleObject(const Point& pt1,
                 const Point& pt2,
                 const Point& pt3,
                 const Color& color);

  virtual void Draw(const MemDC& dc) const override;

private:
  Point m_pt1;
  Point m_pt2;
  Point m_pt3;
  Color m_color;
};
