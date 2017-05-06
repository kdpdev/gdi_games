#include <games/utils/rectangle_object.h>

#include <engine/core/mem_dc.h>
#include <engine/core/os_primitives.h>

RectangleObject::RectangleObject(const Size& size, const Color& color)
  : m_color(color)
{
  SetBounds(Rect(size));
}

void RectangleObject::Draw(const MemDC& dc) const
{
  const HDC hDC = dc.GetHDC();
  const HBRUSH bkgnBrush = CreateSolidBrush(m_color);
  const RECT rect = ToRect(GetBounds());
  FillRect(hDC, &rect, bkgnBrush);
  DeleteObject(bkgnBrush);
}
