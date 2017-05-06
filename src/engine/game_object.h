#pragma once;

#include <engine/core/actions_registry.h>
#include <engine/core/noncopyable.h>
#include <engine/core/primitives.h>

namespace Engine
{
  namespace Core { class MemDC; }

  class GameObject : private Core::Noncopyable
  {
  public:
    enum PublicEvents
    {
      EVENT_MOUSE_CLICK,
      EVENT_MOUSE_MOVE
    };
    using EventId = unsigned;
    using MemDC = Core::MemDC;
    using Action = Core::Action;
    using ActionsRegistry = Core::ActionsRegistry<EventId>;
    using Coord = Core::Coord;
    using Length = Core::Length;
    using Color = Core::Color;
    using Point = Core::Point;
    using Size = Core::Size;
    using Rect = Core::Rect;
    using Circle = Core::Circle;

  public:
    virtual ~GameObject() = default;

    virtual void Draw(const MemDC& dc) const = 0;

    Point GetPos() const;
    void SetPos(const Point& pos);

    Rect GetBounds() const;

    bool OnMouseClick(const Point& pos);
    bool OnMouseMove(const Point& pos);

    void SetEventHandler(EventId eventId, Action action);

  protected:
    virtual bool HandleMouseClick(const Point& pos);
    virtual bool HandleMouseMove(const Point& pos);

    void SetBounds(const Rect& bounds);

  protected:
    bool Signal(EventId eventId);

  private:
    Rect m_bounds;
    ActionsRegistry m_actions;
  };
}
