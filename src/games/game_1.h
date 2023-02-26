#pragma once

#include <engine/base_game.h>

class Game1 : public Engine::BaseGame
{
public:
  Game1(const MemDC& dc, const Rect& bounds, const Color& bkgnColor);
};
