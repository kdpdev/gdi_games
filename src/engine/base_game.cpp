#include <engine/base_game.h>

#include <engine/core/err.h>
#include <engine/core/mem_dc.h>

namespace Engine
{
  BaseGame::BaseGame(const MemDC& dc, const Rect& bounds)
    : m_dc(dc)
    , m_bounds(bounds)
    , m_state(State::STOPPED)
    , m_startTime(Clock::now())
  {
  }

  BaseGame::~BaseGame()
  {
    m_processors.clear();
    m_objects.clear();
  }

  AbstractGame::State BaseGame::GetState() const
  {
    return m_state;
  }

  bool BaseGame::IsStarted() const
  {
    return m_state == State::STARTED;
  }

  bool BaseGame::Start()
  {
    if (!IsStarted())
    {
      m_state = State::STARTED;
      m_startTime = Clock::now();
    }
    return true;
  }

  bool BaseGame::Stop()
  {
    m_state = State::STOPPED;
    return true;
  }

  bool BaseGame::Draw() const
  {
    //if (!IsStarted())
    //{
    //  return false;
    //}

    const HDC hDC = m_dc.GetHDC();

    IntersectClipRect(hDC, m_bounds.GetLeft(), m_bounds.GetTop(), m_bounds.GetRight(), m_bounds.GetBottom());

    for (auto& obj : m_objects)
    {
      obj->Draw(m_dc);
    }

    SelectClipRgn(hDC, NULL);

    return true;
  }

  bool BaseGame::Process()
  {
    if (!IsStarted())
    {
      return false;
    }
    const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_startTime);
    for (auto& processor : m_processors)
    {
      processor(time);
    }
    return true;
  }

  bool BaseGame::HandleMouseEvent(const Point& mousePos, bool(GameObject::*method)(const Point&))
  {
    if (!IsStarted())
    {
      return false;
    }

    if (!m_bounds.IsInside(mousePos))
    {
      return false;
    }

    for (auto it = m_objects.rbegin(), end = m_objects.rend(); it != end; ++it)
    {
      GameObject* obj = it->get();
      const Rect& objBounds = obj->GetBounds();
      if (!objBounds.IsInside(mousePos))
      {
        continue;
      }
      const Point& pos = Point(mousePos.x - objBounds.topLeft.x, mousePos.y - objBounds.topLeft.y);
      if ((obj->*method)(pos))
      {
        return true;
      }
    }

    return false;
  }

  bool BaseGame::OnMouseClick(const Point& mousePos)
  {
    return HandleMouseEvent(mousePos, &GameObject::OnMouseClick);
  }

  bool BaseGame::OnMouseMove(const Point& mousePos)
  {
    return HandleMouseEvent(mousePos, &GameObject::OnMouseMove);
  }

  void BaseGame::SetEventHandler(EventId eventId, Action action)
  {
    m_actions.Set(eventId, std::move(action));
  }

  bool BaseGame::Signal(EventId eventId)
  {
    return m_actions.Execute(eventId);
  }

  void BaseGame::AddGameObject(UniqueGameObjPtr obj)
  {
    ERR_THROW_IF_NOT(obj, "Object is nullptr.");
    m_objects.push_back(std::move(obj));
  }

  void BaseGame::AddTimeProcessor(TimeProcessor processor)
  {
    ERR_THROW_IF_NOT(processor, "Processor is empty.");
    m_processors.push_back(std::move(processor));
  }

  BaseGame::Action BaseGame::CreateEventEmiter(EventId eventId)
  {
    return [this, eventId]
    {
      Signal(eventId);
    };
  }
}
