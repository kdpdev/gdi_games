#pragma once

#include <engine/game_object.h>

class CrossObject : public Engine::GameObject
{
public:
  explicit CrossObject(Length size);

  virtual void Draw(const MemDC& dc) const override;
};
