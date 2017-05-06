#pragma once;

#include <engine/base_game.h>

class Game2 : public Engine::BaseGame
{
public:
  Game2(const MemDC& dc, const Rect& bounds, const Color& bkgnColor);
};
