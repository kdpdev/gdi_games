#pragma once;

#include <engine/abstract_game.h>
#include <engine/game_object.h>

#include <engine/core/actions_registry.h>
#include <engine/core/time_processor.h>

#include <chrono>
#include <memory>
#include <vector>

namespace Engine
{
  namespace Core { class MemDC; }

  class BaseGame : public AbstractGame
  {
  public:
    enum PublicEvents
    {
      EVENT_NEXT_GAME,
      EVENT_EXIT_GAME
    };
    using EventId = unsigned;
    using MemDC = Core::MemDC;
    using Action = Core::Action;
    using TimeProcessor = Core::TimeProcessor;
    using ActionsRegistry = Core::ActionsRegistry<EventId>;
    using Coord = Core::Coord;
    using Length = Core::Length;
    using Color = Core::Color;
    using Point = Core::Point;
    using Size = Core::Size;
    using Rect = Core::Rect;
    using Circle = Core::Circle;
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;
    using UniqueGameObjPtr = std::unique_ptr<GameObject>;

  public:
    BaseGame(const MemDC& dc, const Rect& bounds);
    virtual ~BaseGame();

    virtual State GetState() const override;
    virtual bool Start() override;
    virtual bool Stop() override;
    virtual bool Draw() const override;
    virtual bool Process() override;
    virtual bool OnMouseClick(const Point& mousePos) override;
    virtual bool OnMouseMove(const Point& mousePos) override;

    void SetEventHandler(EventId eventId, Action action);

  protected:
    bool HandleMouseEvent(const Point& mousePos, bool(GameObject::*method)(const Point&));

    bool IsStarted() const;
    bool Signal(EventId eventId);
    void AddGameObject(UniqueGameObjPtr obj);
    void AddTimeProcessor(TimeProcessor processor);
    Action CreateEventEmiter(EventId eventId);

  private:
    using GameObjectsList = std::vector<UniqueGameObjPtr>;
    using TimeProcessorsList = std::vector<TimeProcessor>;

    const MemDC& m_dc;
    const Rect m_bounds;
    State m_state;
    TimePoint m_startTime;
    ActionsRegistry m_actions;
    GameObjectsList m_objects;
    TimeProcessorsList m_processors;
  };
}
