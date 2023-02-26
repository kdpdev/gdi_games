#pragma once

namespace Engine
{
  namespace Core
  {
    class Noncopyable
    {
    protected:
      Noncopyable() = default;
      ~Noncopyable() = default;
      Noncopyable(const Noncopyable&) = delete;
      Noncopyable& operator = (const Noncopyable&) = delete;
    };
  }
}
