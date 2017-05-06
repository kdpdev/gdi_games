#include <games/game_2.h>

#include <games/utils/circle_object.h>
#include <games/utils/cross_object.h>
#include <games/utils/rectangle_object.h>
#include <games/utils/movements.h>

#include <engine/core/os_primitives.h>

#include <algorithm>


Game2::Game2(const MemDC& dc, const Rect& bounds, const Color& bkgnColor)
  : BaseGame(dc, bounds)
{
  auto bkgn = std::make_unique<RectangleObject>(bounds.size, bkgnColor);
  bkgn->SetPos(bounds.topLeft);

  const auto crossSize = (std::min)(bounds.GetWidth(), bounds.GetHeight()) / 15;
  auto cross = std::make_unique<CrossObject>(crossSize);
  cross->SetPos(Point(bounds.GetRight() - crossSize, bounds.GetTop()));
  cross->SetEventHandler(Engine::GameObject::EVENT_MOUSE_CLICK, CreateEventEmiter(EVENT_EXIT_GAME));

  const auto stableCircleR = (std::min)(bounds.GetWidth(), bounds.GetHeight()) / 5;
  auto stableCircle = std::make_unique<CircleObject>(stableCircleR, RGB(255, 255, 0));
  stableCircle->SetPos(bounds.topLeft);
  stableCircle->SetEventHandler(Engine::GameObject::EVENT_MOUSE_CLICK, CreateEventEmiter(EVENT_NEXT_GAME));

  const auto circleR = (std::min)(bounds.GetWidth(), bounds.GetHeight()) / 10;
  auto circle = std::make_unique<CircleObject>(circleR, RGB(255, 255, 0));
  circle->SetPos(bounds.topLeft);
  circle->SetEventHandler(Engine::GameObject::EVENT_MOUSE_CLICK, CreateEventEmiter(EVENT_NEXT_GAME));

  const unsigned secondsPerPerimeter = 5;
  const auto indent = 2;
  const Rect movementRect = Rect(bounds.GetLeft() + indent,
                                 bounds.GetTop() + indent,
                                 bounds.GetWidth() - indent * 2 - circleR * 2,
                                 bounds.GetHeight() - indent * 2 - circleR * 2);
  const auto perimeter = 2 * (movementRect.GetWidth() + movementRect.GetHeight());
  const double squareSpeed = double(perimeter) / secondsPerPerimeter;
  auto circleMovement = CreateRectangularMovement(*circle, squareSpeed, movementRect);

  AddGameObject(std::move(bkgn));
  AddGameObject(std::move(stableCircle));
  AddGameObject(std::move(circle));
  AddGameObject(std::move(cross));

  AddTimeProcessor(std::move(circleMovement));
}
