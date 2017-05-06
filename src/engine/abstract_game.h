#pragma once;

#include <engine/core/noncopyable.h>
#include <engine/core/primitives.h>

namespace Engine
{
  class AbstractGame : private Core::Noncopyable
  {
  public:
    enum class State
    {
      STARTED,
      STOPPED
    };
    using Coord = Core::Coord;
    using Lengt = Core::Length;
    using Color = Core::Color;
    using Point = Core::Point;
    using Size = Core::Size;
    using Rect = Core::Rect;

  public:
    virtual ~AbstractGame() = default;

    virtual State GetState() const = 0;
    virtual bool Start() = 0;
    virtual bool Stop() = 0;
    virtual bool Draw() const = 0;
    virtual bool Process() = 0;
    virtual bool OnMouseClick(const Point& mousePos) = 0;
    virtual bool OnMouseMove(const Point& mousePos) = 0;
  };
}
