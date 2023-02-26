#pragma once

#include <engine/core/primitives.h>

namespace Engine
{
  namespace Core
  {
    inline
    Point ToPoint(const POINT& p) noexcept
    {
      return Point(static_cast<Coord>(p.x),
                   static_cast<Coord>(p.y));
    }

    inline
    POINT ToPoint(const Point& p) noexcept
    {
      const POINT result =
      {
        static_cast<LONG>(p.x),
        static_cast<LONG>(p.y)
      };
      return result;
    }

    inline
    Size ToSize(const SIZE& size) noexcept
    {
      return Size(static_cast<Length>(size.cx),
                  static_cast<Length>(size.cy));
    }

    inline
    SIZE ToSize(const Size& size) noexcept
    {
      const SIZE result =
      {
        static_cast<LONG>(size.width),
        static_cast<LONG>(size.height)
      };
      return result;
    }

    inline
    Rect ToRect(const RECT& rect) noexcept
    {
      return Rect(static_cast<Coord>(rect.left),
                  static_cast<Coord>(rect.top),
                  static_cast<Length>(rect.right - rect.left),
                  static_cast<Length>(rect.bottom - rect.top));
    }

    inline
    RECT ToRect(const Rect& rect) noexcept
    {
      const RECT result =
      {
        static_cast<LONG>(rect.GetLeft()),
        static_cast<LONG>(rect.GetTop()),
        static_cast<LONG>(rect.GetRight()),
        static_cast<LONG>(rect.GetBottom())
      };
      return result;
    }
  }
}
