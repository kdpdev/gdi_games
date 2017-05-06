#pragma once;

#include <engine/core/primitives.h>
#include <engine/core/time_processor.h>

namespace Engine { class GameObject; }

Engine::Core::TimeProcessor CreateCircularMovement(
  Engine::GameObject& obj,
  double rotatesPerSecond,
  const Engine::Core::Circle& circle);

Engine::Core::TimeProcessor CreateRectangularMovement(
  Engine::GameObject& obj,
  double pixelsPerSecond,
  const Engine::Core::Rect& rect);

Engine::Core::TimeProcessor CreateHorizontalMovement(
  Engine::GameObject& obj,
  double pixelsPerSecond,
  Engine::Core::Coord left,
  Engine::Core::Coord right);
