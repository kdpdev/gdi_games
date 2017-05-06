#include <games/utils/circle_object.h>

#include <engine/core/mem_dc.h>
#include <engine/core/os_primitives.h>


CircleObject::CircleObject(Length radius, const Color& color)
  : m_color(color)
{
  SetBounds(Circle(Point(), radius).GetBounds());
}

void CircleObject::Draw(const MemDC& dc) const
{
  const HDC hDC = dc.GetHDC();

  const HBRUSH hBkgnBrush = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
  const HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(hDC, hBkgnBrush));

  const HPEN hPen = CreatePen(PS_SOLID, 3, m_color);
  const HPEN hOldPen = static_cast<HPEN>(SelectObject(hDC, hPen));

  const RECT rect = ToRect(Circle(GetBounds()).GetBounds());
  Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);

  SelectObject(hDC, hOldBrush);
  SelectObject(hDC, hOldPen);

  DeleteObject(hPen);
}

bool CircleObject::HandleMouseClick(const Point& pos)
{
  Circle circle(GetBounds());
  circle.center.x = circle.radius;
  circle.center.y = circle.radius;
  return circle.IsInside(pos);
}

bool CircleObject::HandleMouseMove(const Point& pos)
{
  return Circle(GetBounds()).IsInside(pos);
}
