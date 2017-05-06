#include <games/utils/cross_object.h>

#include <engine/core/mem_dc.h>
#include <engine/core/os_primitives.h>

#include <algorithm>

CrossObject::CrossObject(Length size)
{
  SetBounds(Rect(Size(size, size)));
}

void CrossObject::Draw(const MemDC& dc) const
{
  const HDC hDC = dc.GetHDC();

  const HBRUSH hBkgnBrush = CreateSolidBrush(RGB(255, 255, 255));
  const RECT btnRect = ToRect(GetBounds());
  FillRect(hDC, &btnRect, hBkgnBrush);
  DeleteObject(hBkgnBrush);

  const HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
  const HPEN hOldPen = static_cast<HPEN>(SelectObject(hDC, hPen));

  const LONG crossSizeDiff = (std::min)(btnRect.right - btnRect.left, btnRect.bottom - btnRect.top) * 20 / 100;
  RECT crossRect = btnRect;
  InflateRect(&crossRect, -crossSizeDiff, -crossSizeDiff);
  MoveToEx(hDC, crossRect.left, crossRect.top, 0);
  LineTo(hDC, crossRect.right, crossRect.bottom);
  MoveToEx(hDC, crossRect.left, crossRect.bottom, 0);
  LineTo(hDC, crossRect.right, crossRect.top);

  SelectObject(hDC, hOldPen);
  DeleteObject(hPen); 
}
