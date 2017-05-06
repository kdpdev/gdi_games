#pragma once;

#include <chrono>
#include <functional>

namespace Engine
{
  namespace Core
  {
    using TimeProcessor = std::function<void(std::chrono::milliseconds)>;
  }
}
