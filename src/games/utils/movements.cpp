#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <games/utils/movements.h>

#include <engine/game_object.h>

#include <engine/core/err.h>

#include <cmath>

using namespace Engine::Core;

TimeProcessor CreateCircularMovement(
  Engine::GameObject& obj,
  double rotatesPerSecond,
  const Circle& circle)
{
  const auto rpms = rotatesPerSecond / 1000;
  return [&obj, rpms, circle](auto time) mutable
  {
    double intpart = 0;
    const double rotateFract = std::modf(rpms * time.count(), &intpart);
    const double rad = rotateFract * 2 * M_PI - M_PI / 2;
    const double x = cos(rad) * circle.radius + circle.center.x;
    const double y = sin(rad) * circle.radius + circle.center.y;
    obj.SetPos({ static_cast<Coord>(x), static_cast<Coord>(y) });
  };
}

TimeProcessor CreateRectangularMovement(
  Engine::GameObject& obj,
  double pixelsPerSecond,
  const Rect& rect)
{
  const auto ppms = pixelsPerSecond / 1000;
  return [&obj, ppms, rect](auto time) mutable
  {
    const auto passedDistance = static_cast<Length>(ppms * time.count());
    const auto halfPerimeter = rect.GetWidth() + rect.GetHeight();
    const auto perimeter = halfPerimeter * 2;
    const auto path = passedDistance % perimeter;

    Point pos;

    if (path < halfPerimeter)
    {
      if (path < rect.GetWidth())
      {
        pos.x = rect.GetLeft() + path;
        pos.y = rect.GetTop();
      }
      else
      {
        pos.x = rect.GetLeft() + rect.GetWidth();
        pos.y = rect.GetTop() + (path - rect.GetWidth());
      }
    }
    else
    {
      const auto bottomPath = path - halfPerimeter;
      if (bottomPath < rect.GetWidth())
      {
        pos.x = rect.GetRight() - bottomPath;
        pos.y = rect.GetBottom();
      }
      else
      {
        pos.x = rect.GetLeft();
        pos.y = rect.GetBottom() - (bottomPath - rect.GetWidth());
      }
    }

    obj.SetPos(pos);
  };
}

TimeProcessor CreateHorizontalMovement(
  Engine::GameObject& obj,
  double pixelsPerSecond,
  Coord left,
  Coord right)
{
  ERR_THROW_IF(left > right, "Incorrect bounds.");
  const auto ppms = pixelsPerSecond / 1000;
  return [&obj, ppms, left, right](auto time) mutable
  {
    const Length borderedDistance = static_cast<Length>(right - left);
    const Length passedDistance = static_cast<Length>(ppms * time.count());
    const Length path = passedDistance % borderedDistance;

    Coord xPos = 0;
    if ((passedDistance / borderedDistance) % 2)
    {
      xPos = right - path;
    }
    else
    {
      xPos = left + path;
    }

    obj.SetPos({ xPos, obj.GetBounds().GetTop() });
  };
}
