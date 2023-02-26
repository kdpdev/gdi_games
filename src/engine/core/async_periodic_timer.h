#pragma once

#include <engine/core/action.h>
#include <engine/core/noncopyable.h>

#include <chrono>
#include <memory>

namespace Engine
{
  namespace Core
  {
    class AsyncPeriodicTimer : private Noncopyable
    {
    public:
      AsyncPeriodicTimer(std::chrono::milliseconds period, Action action);
      ~AsyncPeriodicTimer();

      void Start();
      void Stop();

    private:
      class Impl;
      std::unique_ptr<Impl> m_impl;
    };
  }
}
