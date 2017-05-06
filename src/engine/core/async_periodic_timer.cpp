#include <engine/core/async_periodic_timer.h>
#include <engine/core/err.h>

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <thread>

namespace Engine
{
namespace Core
{
  class AsyncPeriodicTimer::Impl : private Noncopyable
  {
  public:
    Impl(std::chrono::milliseconds period, Action action)
      : m_period(period)
      , m_action(wrapAction(std::move(action)))
      , m_runFlag(false)
    {
    }

    ~Impl()
    {
      Stop();
    }

    void Start()
    {
      std::unique_lock<std::mutex> guard(m_mutex);
      SynchronizedStop();
      try
      {
        m_runFlag = true;
        m_thread = std::thread([this]
        {
          threadFunc();
        });
      }
      catch (...)
      {
        m_runFlag = false;
        throw;
      }
    }

    void Stop()
    {
      std::unique_lock<std::mutex> guard(m_mutex);
      SynchronizedStop();
    }

  private:
    void SynchronizedStop()
    {
      m_runFlag = false;
      if (m_thread.joinable())
      {
        m_wakeupEvent.notify_all();
        m_thread.join();
      }
    }

    void threadFunc()
    {
      std::mutex mutex;
      std::unique_lock<std::mutex> guard(mutex);
      while (m_runFlag)
      {
        m_wakeupEvent.wait_for(guard, m_period, [this]
        {
          return !m_runFlag.load();
        });

        if (m_runFlag)
        {
          m_action();
        }
      }
    }

  private:
    static Action wrapAction(Action action)
    {
      ERR_THROW_IF_NOT(action, "Action is empty.");
      return [action = std::move(action)]
      {
        try
        {
          action();
        }
        catch (...)
        {
          assert(!"Timer exception.");
        }
      };
    }

  private:
    const std::chrono::milliseconds m_period;
    const Action m_action;
    std::thread m_thread;
    std::atomic_bool m_runFlag;
    std::condition_variable m_wakeupEvent;
    std::mutex m_mutex;
  };

  AsyncPeriodicTimer::AsyncPeriodicTimer(std::chrono::milliseconds period, Action action)
    : m_impl(std::make_unique<Impl>(period, std::move(action)))
  {
  }

  AsyncPeriodicTimer::~AsyncPeriodicTimer()
  {
  }

  void AsyncPeriodicTimer::Start()
  {
    m_impl->Start();
  }

  void AsyncPeriodicTimer::Stop()
  {
    m_impl->Stop();
  }
}
}
