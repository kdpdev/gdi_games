#include <games/utils/triangle_object.h>

#include <engine/core/mem_dc.h>

#include <algorithm>


TriangleObject::TriangleObject(const Point& pt1,
                               const Point& pt2,
                               const Point& pt3,
                               const Color& color)
  : m_color(color)
{
  const Coord left   = (std::min)((std::min)(pt1.x, pt2.x), pt3.x);
  const Coord right  = (std::max)((std::max)(pt1.x, pt2.x), pt3.x);
  const Coord top    = (std::min)((std::min)(pt1.y, pt2.y), pt3.y);
  const Coord bottom = (std::max)((std::max)(pt1.y, pt2.y), pt3.y);

  m_pt1.x = pt1.x - left;
  m_pt2.x = pt2.x - left;
  m_pt3.x = pt3.x - left;

  m_pt1.y = pt1.y - top;
  m_pt2.y = pt2.y - top;
  m_pt3.y = pt3.y - top;

  SetBounds(Rect(left, top, static_cast<Length>(right - left), static_cast<Length>(bottom - top)));
}

void TriangleObject::Draw(const MemDC& dc) const
{
  const HDC hDC = dc.GetHDC();

  const HPEN hCrossPen = CreatePen(PS_SOLID, 3, m_color);
  const HPEN hOldPen = static_cast<HPEN>(SelectObject(hDC, hCrossPen));

  const Point topLeft = GetBounds().topLeft;

  MoveToEx(hDC, topLeft.x + m_pt1.x, topLeft.y + m_pt1.y, 0);
  LineTo(hDC, topLeft.x + m_pt2.x, topLeft.y + m_pt2.y);
  LineTo(hDC, topLeft.x + m_pt3.x, topLeft.y + m_pt3.y);
  LineTo(hDC, topLeft.x + m_pt1.x, topLeft.y + m_pt1.y);

  SelectObject(hDC, hOldPen);
  DeleteObject(hCrossPen); 
}
