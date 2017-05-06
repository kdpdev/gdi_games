#include <games/game_4.h>

#include <games/utils/cross_object.h>
#include <games/utils/movements.h>
#include <games/utils/rectangle_object.h>

#include <engine/core/os_primitives.h>

#include <algorithm>


Game4::Game4(const MemDC& dc, const Rect& bounds, const Color& bkgnColor)
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

  AddGameObject(std::move(bkgn));

  for (Length i = 0; i < bounds.GetHeight() / 2; ++i)
  {
    const Length squareSize = 2;
    const Coord squareY = bounds.GetTop() + i * squareSize;
    auto square = std::make_unique<RectangleObject>(Size(squareSize, squareSize), RGB(0, 255, 0));
      square->SetPos(Point(bounds.topLeft.x, squareY));

    const double squareSpeed = (10 + i) * 5;
    const Coord leftBorder = bounds.GetLeft();
    const Coord rightBorder = bounds.GetRight() - squareSize;
    auto squareMovement = CreateHorizontalMovement(*square, squareSpeed, leftBorder, rightBorder);

    AddGameObject(std::move(square));
    AddTimeProcessor(std::move(squareMovement));
  }

  for (Length i = 0; i < bounds.GetHeight(); ++i)
  {
    const Length squareSize = 1;
    const Coord squareY = bounds.GetTop() + i;
    auto square = std::make_unique<RectangleObject>(Size(squareSize, squareSize), RGB(255, 0, 0));
      square->SetPos(Point(bounds.topLeft.x, squareY));

    const double squareSpeed = (10 + i) * 3;
    const Coord leftBorder = bounds.GetLeft() + bounds.GetWidth() / 4;
    const Coord rightBorder = bounds.GetRight() - squareSize - bounds.GetWidth() / 4;
    auto squareMovement = CreateHorizontalMovement(*square, squareSpeed, leftBorder, rightBorder);

    AddGameObject(std::move(square));
    AddTimeProcessor(std::move(squareMovement));
  }

  AddGameObject(std::move(cross));
}
