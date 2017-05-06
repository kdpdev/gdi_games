#include <games/game_3.h>

#include <games/utils/circle_object.h>
#include <games/utils/cross_object.h>
#include <games/utils/movements.h>
#include <games/utils/rectangle_object.h>
#include <games/utils/triangle_object.h>

#include <engine/core/os_primitives.h>

#include <algorithm>

Game3::Game3(const MemDC& dc, const Rect& bounds, const Color& bkgnColor)
  : BaseGame(dc, bounds)
{
  auto bkgn = std::make_unique<RectangleObject>(bounds.size, bkgnColor);
  bkgn->SetPos(bounds.topLeft);

  const Length crossSize = (std::min)(bounds.GetWidth(), bounds.GetHeight()) / 15;
  auto cross = std::make_unique<CrossObject>(crossSize);
  cross->SetPos(Point(bounds.GetRight() - crossSize, bounds.GetTop()));
  cross->SetEventHandler(Engine::GameObject::EVENT_MOUSE_CLICK, CreateEventEmiter(EVENT_EXIT_GAME));

  const Point pt1 = Point(0, 0);
  const Point pt2 = Point(20, 30);
  const Point pt3 = Point(10, 50);
  auto triangle = std::make_unique<TriangleObject>(pt1, pt2, pt3, RGB(255, 255, 0));

  auto circle = std::make_unique<CircleObject>(15, RGB(200, 0, 0));
  circle->SetEventHandler(Engine::GameObject::EVENT_MOUSE_CLICK, CreateEventEmiter(EVENT_NEXT_GAME));

  const double rotatesPerSecond = 0.3;
  const Rect& triangleBounds = triangle->GetBounds();
  const Length triangleMovementR = (std::min)(bounds.GetWidth() - triangleBounds.GetWidth(), bounds.GetHeight() - triangleBounds.GetHeight()) / 2;
  const Point triangleMovementCenter(bounds.GetCenter().x - triangleBounds.GetWidth() / 2, bounds.GetCenter().y - triangleBounds.GetHeight() / 2);
  auto triangleMovement = CreateCircularMovement(*triangle, rotatesPerSecond, Circle(triangleMovementCenter, triangleMovementR));

  auto trianglePath = std::make_unique<CircleObject>(triangleMovementR, RGB(0, 0, 255));
  const Point trianglePathPos(bounds.GetCenter().x - triangleMovementR, bounds.GetCenter().y - triangleMovementR);
  trianglePath->SetPos(trianglePathPos);

  auto circleMovement = CreateCircularMovement(*circle, rotatesPerSecond / 4, Circle(Point(bounds.GetLeft() + 30, bounds.GetTop() + 30), 40));

  AddGameObject(std::move(bkgn));
  AddGameObject(std::move(trianglePath));
  AddGameObject(std::move(triangle));
  AddGameObject(std::move(circle));
  AddGameObject(std::move(cross));

  AddTimeProcessor(std::move(triangleMovement));
  AddTimeProcessor(std::move(circleMovement));
}
