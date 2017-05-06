#include <engine/game_object.h>

namespace Engine
{
  GameObject::Point GameObject::GetPos() const
  {
    return m_bounds.topLeft;
  }

  void GameObject::SetPos(const Point& pos)
  {
    m_bounds.topLeft = pos;
  }

  GameObject::Rect GameObject::GetBounds() const
  {
    return m_bounds;
  }

  void GameObject::SetBounds(const Rect& bounds)
  {
    m_bounds = bounds;
  }

  bool GameObject::OnMouseClick(const Point& pos)
  {
    if (HandleMouseClick(pos))
    {
      Signal(EVENT_MOUSE_CLICK);
      return true;
    }
    return false;
  }

  bool GameObject::OnMouseMove(const Point& pos)
  {
    if (!HandleMouseMove(pos))
    {
      Signal(EVENT_MOUSE_MOVE);
      return true;
    }
    return false;
  }

  bool GameObject::HandleMouseClick(const Point&)
  {
    return true;
  }

  bool GameObject::HandleMouseMove(const Point&)
  {
    return true;
  }

  void GameObject::SetEventHandler(EventId eventId, Action action)
  {
    m_actions.Set(eventId, std::move(action));
  }

  bool GameObject::Signal(EventId eventId)
  {
    return m_actions.Execute(eventId);
  }
}
