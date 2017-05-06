#include <games/game_1.h>

#include <games/utils/cross_object.h>
#include <games/utils/movements.h>
#include <games/utils/rectangle_object.h>

#include <engine/core/os_primitives.h>

#include <algorithm>

Game1::Game1(const MemDC& dc, const Rect& bounds, const Color& bkgnColor)
  : BaseGame(dc, bounds)
{
  auto bkgn = std::make_unique<RectangleObject>(bounds.size, bkgnColor);
  bkgn->SetPos(bounds.topLeft);

  const Length crossSize = (std::min)(bounds.GetWidth(), bounds.GetHeight()) / 15;
  auto cross = std::make_unique<CrossObject>(crossSize);
  cross->SetPos(Point(bounds.GetRight() - crossSize, bounds.GetTop()));
  cross->SetEventHandler(Engine::GameObject::EVENT_MOUSE_CLICK, CreateEventEmiter(EVENT_EXIT_GAME));

  const Length squareSize = (std::min)(bounds.GetWidth(), bounds.GetHeight()) / 8;
  const Coord squareY = bounds.GetTop() + (bounds.GetHeight() - squareSize) / 2;
  auto square = std::make_unique<RectangleObject>(Size(squareSize, squareSize), RGB(255, 255, 0));
  square->SetPos(Point(bounds.GetLeft(), squareY));
  square->SetEventHandler(Engine::GameObject::EVENT_MOUSE_CLICK, CreateEventEmiter(EVENT_NEXT_GAME));

  const unsigned secondsPerScreen = 5;
  const double squareSpeed = double(bounds.GetWidth()) / secondsPerScreen;
  const Coord leftBorder = bounds.GetLeft();
  const Coord rightBorder = bounds.GetRight() - squareSize;
  auto squareMovement = CreateHorizontalMovement(*square, squareSpeed, leftBorder, rightBorder);

  AddGameObject(std::move(bkgn));
  AddGameObject(std::move(square));
  AddGameObject(std::move(cross));

  AddTimeProcessor(std::move(squareMovement));
}
