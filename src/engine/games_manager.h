#pragma once;

#include <engine/core/action.h>
#include <engine/core/noncopyable.h>
#include <engine/core/primitives.h>

#include <chrono>
#include <memory>

namespace Engine
{
  namespace Core { class MemDC; }
  class AbstractGame;

  class GamesManager final : private Core::Noncopyable
  {
  public:
    using UniqueGamePtr = std::unique_ptr<AbstractGame>;

  public:
    GamesManager();
    ~GamesManager();

    void InitScreen(const Core::Size& size);
    const Core::MemDC& GetScreen() const && = delete;
    const Core::MemDC& GetScreen() const &;
    void SetUpdateScreenAction(Core::Action updateAction);

    void StartProcessingLoop(std::chrono::milliseconds period);
    void StartRenderingLoop(std::chrono::milliseconds period);

    void PushGame(UniqueGamePtr game);
    void PopGame();
    bool StartTopGame();
    bool StopTopGame();

    bool OnMouseClick(const Core::Point& mousePos);
    bool OnMouseMove(const Core::Point& mousePos);

  private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
  };
}
