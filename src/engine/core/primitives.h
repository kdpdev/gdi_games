#pragma once;

#include <cstdint>
#include <algorithm>

#include <windows.h>

namespace Engine
{
  namespace Core
  {
    using Color = COLORREF;
    using Coord = std::int16_t;
    using Length = std::uint32_t;

    struct Point
    {
      Coord x;
      Coord y;

      Point() noexcept
        : x(0)
        , y(0)
      {
      }

      Point(Coord theX, Coord theY) noexcept
        : x(theX)
        , y(theY)
      {
      }
    };

    struct Size
    {
      Length width;
      Length height;

      Size() noexcept
        : width(0)
        , height(0)
      {
      }

      Size(Length theWidth, Length theHeight) noexcept
        : width(theWidth)
        , height(theHeight)
      {
      }
    };

    struct Rect
    {
      Point topLeft;
      Size size;

      Rect() noexcept
      {
      }

      explicit Rect(const Size& size) noexcept
        : size(size)
      {
      }

      Rect(const Point& theTopLeft, const Size& theSize) noexcept
        : topLeft(theTopLeft)
        , size(theSize)
      {
      }

      Rect(Coord x, Coord y, Length width, Length height) noexcept
        : topLeft(x, y)
        , size(width, height)
      {
      }

      Point GetCenter() const noexcept
      {
        return Point(topLeft.x + size.width / 2, topLeft.y + size.height / 2);
      }

      Coord GetLeft() const noexcept
      {
        return topLeft.x;
      }

      Coord GetRight() const noexcept
      {
        return topLeft.x + size.width;
      }

      Coord GetTop() const noexcept
      {
        return topLeft.y;
      }

      Coord GetBottom() const noexcept
      {
        return topLeft.y + size.height;
      }

      Length GetWidth() const noexcept
      {
        return size.width;
      }

      Length GetHeight() const noexcept
      {
        return size.height;
      }

      bool IsInside(const Point& p) const noexcept
      {
        return GetLeft() <= p.x
          && GetRight() >= p.x
          && GetTop() <= p.y
          && GetBottom() >= p.y
          ;
      }
    };

    struct Circle
    {
      Point center;
      Length radius;

      Circle(const Point& theCenter, Length theRadius) noexcept
        : center(theCenter)
        , radius(theRadius)
      {
      }

      Circle(const Rect& bounds) noexcept
        : center(bounds.GetCenter())
        , radius((std::min)(bounds.GetWidth(), bounds.GetHeight()) / 2)
      {
      }

      Rect GetBounds() const noexcept
      {
        return Rect(center.x - radius, center.y - radius, radius * 2, radius * 2);
      }

      bool IsInside(const Point& p) const noexcept
      {
        const Point relPos = Point(p.x - center.x, p.y - center.y);
        return static_cast<Length>(relPos.x * relPos.x + relPos.y* relPos.y) <= radius * radius;
      }
    };
  }
}
