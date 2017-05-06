#include <engine/games_manager.h>
#include <engine/abstract_game.h>

#include <engine/core/async_periodic_timer.h>
#include <engine/core/err.h>
#include <engine/core/mem_dc.h>

#include <deque>
#include <mutex>

namespace Engine
{
  class GamesManager::Impl : private Core::Noncopyable
  {
    using UniqueGamePtr = GamesManager::UniqueGamePtr;
    using GamesList = std::deque<UniqueGamePtr>;
    using MemDC = Core::MemDC;
    using Action = Core::Action;
    using AsyncPeriodicTimer = Core::AsyncPeriodicTimer;
    using Guard = std::recursive_mutex;
    using ScopedLock = std::unique_lock<Guard>;

  public:
    Impl()
    {
      m_screenUpdater = []{};
    }

    ~Impl()
    {
      m_processingTimer.reset();
      m_renderingTimer.reset();
      const ScopedLock gamesLock(m_gamesGuard);
      m_games.clear();
    }

    void InitScreen(const Core::Size& size)
    {
      const ScopedLock screenLock(m_screenGuard);
      if (!m_screenDC.get())
      {
        m_screenDC = std::make_unique<MemDC>();
      }
      m_screenDC->Resize(size);
    }

    const MemDC& GetScreen() const &
    {
      const ScopedLock screenLock(m_screenGuard);
      ERR_THROW_IF_NOT(m_screenDC, "Screen DC is not initialized.");
      return *m_screenDC;
    }

    void SetUpdateScreenAction(Action updateAction)
    {
      const ScopedLock screenLock(m_screenGuard);
      if (updateAction)
      {
        m_screenUpdater = std::move(updateAction);
      }
      else
      {
        m_screenUpdater = [] {};
      }
    }

    void StartProcessingLoop(std::chrono::milliseconds period)
    {
      m_processingTimer.reset();
      m_processingTimer = CreateProcessingTimer(period);
      m_processingTimer->Start();
    }

    void StartRenderingLoop(std::chrono::milliseconds period)
    {
      m_renderingTimer.reset();
      m_renderingTimer = CreateRenderingTimer(period);
      m_renderingTimer->Start();
    }

    AbstractGame* TopGame() const && = delete;
    AbstractGame* TopGame() const &
    {
      const ScopedLock gamesLock(m_gamesGuard);
      return m_games.empty() ? nullptr : m_games.back().get();
    }

    void PushGame(UniqueGamePtr game)
    {
      ERR_THROW_IF_NOT(game, "Game is nullptr.");
      const ScopedLock gamesLock(m_gamesGuard);
      m_games.push_back(std::move(game));
    }

    void PopGame()
    {
      const ScopedLock gamesLock(m_gamesGuard);
      ERR_THROW_IF(m_games.empty(), "There is no any game.");
      m_games.pop_back();
    }

    bool StartTopGame()
    {
      const ScopedLock gamesLock(m_gamesGuard);
      if (AbstractGame* game = TopGame())
      {
        return game->Start();
      }
      return false;
    }

    bool StopTopGame()
    {
      const ScopedLock gamesLock(m_gamesGuard);
      if (AbstractGame* game = TopGame())
      {
        return game->Stop();
      }
      return false;
    }

    bool OnMouseClick(const Core::Point& mousePos)
    {
      const ScopedLock gamesLock(m_gamesGuard);
      if (AbstractGame* game = TopGame())
      {
        m_games.back()->OnMouseClick(mousePos);
      }
      return true;
    }

    bool OnMouseMove(const Core::Point& mousePos)
    {
      const ScopedLock gamesLock(m_gamesGuard);
      if (AbstractGame* game = TopGame())
      {
        m_games.back()->OnMouseMove(mousePos);
      }
      return true;
    }

    void OnDrawEvent()
    {
      const ScopedLock gamesLock(m_gamesGuard);
      const ScopedLock screenLock(m_screenGuard);
      for (auto& game : m_games)
      {
        game->Draw();
      }
      m_screenUpdater();
    }

    void OnProcessEvent()
    {
      const ScopedLock gamesLock(m_gamesGuard);
      for (auto& game : m_games)
      {
        game->Process();
      }
    }

  private:
    std::unique_ptr<AsyncPeriodicTimer> CreateRenderingTimer(std::chrono::milliseconds period)
    {
      return std::make_unique<AsyncPeriodicTimer>(period, [this]
      {
        OnDrawEvent();
      });
    }

    std::unique_ptr<AsyncPeriodicTimer> CreateProcessingTimer(std::chrono::milliseconds period)
    {
      return std::make_unique<AsyncPeriodicTimer>(period, [this]
      {
        OnProcessEvent();
      });
    }

  private:
    Action m_screenUpdater;
    std::unique_ptr<MemDC> m_screenDC;
    GamesList m_games;
    mutable Guard m_gamesGuard;
    mutable Guard m_screenGuard;
    std::unique_ptr<AsyncPeriodicTimer> m_renderingTimer;
    std::unique_ptr<AsyncPeriodicTimer> m_processingTimer;
  };


  GamesManager::GamesManager()
    : m_impl(std::make_unique<Impl>())
  {
  }

  GamesManager::~GamesManager()
  {
  }

  void GamesManager::InitScreen(const Core::Size& size)
  {
    m_impl->InitScreen(size);
  }

  const Core::MemDC& GamesManager::GetScreen() const &
  {
    return m_impl->GetScreen();
  }

  void GamesManager::SetUpdateScreenAction(Core::Action updateAction)
  {
    m_impl->SetUpdateScreenAction(std::move(updateAction));
  }

  void GamesManager::StartProcessingLoop(std::chrono::milliseconds period)
  {
    m_impl->StartProcessingLoop(period);
  }

  void GamesManager::StartRenderingLoop(std::chrono::milliseconds period)
  {
    m_impl->StartRenderingLoop(period);
  }

  void GamesManager::PushGame(UniqueGamePtr game)
  {
    m_impl->PushGame(std::move(game));
  }

  void GamesManager::PopGame()
  {
    m_impl->PopGame();
  }

  bool GamesManager::StartTopGame()
  {
    return m_impl->StartTopGame();
  }

  bool GamesManager::StopTopGame()
  {
    return m_impl->StopTopGame();
  }

  bool GamesManager::OnMouseClick(const Core::Point& mousePos)
  {
    return m_impl->OnMouseClick(mousePos);
  }

  bool GamesManager::OnMouseMove(const Core::Point& mousePos)
  {
    return m_impl->OnMouseMove(mousePos);
  }
}
