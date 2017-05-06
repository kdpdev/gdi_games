#include <games_factory.h>

#include <games/game_1.h>
#include <games/game_2.h>
#include <games/game_3.h>
#include <games/game_4.h>

#include <engine/games_manager.h>
#include <engine/core/mem_dc.h>

namespace
{
  using namespace Engine::Core;

  Engine::Core::Action CreateQuitAppAction()
  {
    return []
    {
      PostQuitMessage(0);
    };
  }
  
  Engine::Core::Action CreatePopGameAction(Engine::GamesManager& gamesManager)
  {
    return [&gamesManager]
    {
      auto& gm = gamesManager;
      gm.StopTopGame();
      gm.PopGame();
      gm.StartTopGame();
    };
  }

  void PushNewGame(Engine::GamesManager& gamesManager, Engine::GamesManager::UniqueGamePtr game)
  {
    //gamesManager.StopTopGame();
    gamesManager.PushGame(std::move(game));
    gamesManager.StartTopGame();
  }

  decltype(auto) CreatePush4thGameAction(Engine::GamesManager& gamesManager)
  {
    return [&gamesManager]
    {
      const Engine::Core::MemDC& screenDC = gamesManager.GetScreen();
      const Size& screenSize = screenDC.GetSize();
      const Size gameSize(screenSize.width - screenSize.width / 5, screenSize.height / 2);
      const Point gamePos(0, screenSize.height / 2);
      auto game = std::make_unique<Game4>(screenDC, Rect(gamePos, gameSize), RGB(0, 0, 0));
      game->SetEventHandler(Game4::EVENT_EXIT_GAME, CreatePopGameAction(gamesManager));
      PushNewGame(gamesManager, std::move(game));
    };
  }

  decltype(auto) CreatePush3rdGameAction(Engine::GamesManager& gamesManager)
  {
    return [&gamesManager]
    {
      const Engine::Core::MemDC& screenDC = gamesManager.GetScreen();
      const Size& screenSize = screenDC.GetSize();
      const Size gameSize = Size(screenSize.width / 2, screenSize.height / 2);
      const Point gamePos((screenSize.width - gameSize.width) / 2, (screenSize.height - gameSize.height) / 2);
      auto game = std::make_unique<Game3>(screenDC, Rect(gamePos, gameSize), RGB(0, 0, 80));
      game->SetEventHandler(Game3::EVENT_NEXT_GAME, CreatePush4thGameAction(gamesManager));
      game->SetEventHandler(Game3::EVENT_EXIT_GAME, CreatePopGameAction(gamesManager));
      PushNewGame(gamesManager, std::move(game));
    };
  }

  decltype(auto) CreatePush2ndGameAction(Engine::GamesManager& gamesManager)
  {
    return [&gamesManager]
    {
      const Engine::Core::MemDC& screenDC = gamesManager.GetScreen();
      const Size& screenSize = screenDC.GetSize();
      const Size gameSize(screenSize.width * 8 / 10, screenSize.height * 8 / 10);
      const Point gamePos((screenSize.width - gameSize.width) / 2, (screenSize.height - gameSize.height) / 2);
      auto game = std::make_unique<Game2>(screenDC, Rect(gamePos, gameSize), RGB(0, 80, 0));
      game->SetEventHandler(Game2::EVENT_EXIT_GAME, CreatePopGameAction(gamesManager));
      game->SetEventHandler(Game2::EVENT_NEXT_GAME, CreatePush3rdGameAction(gamesManager));
      PushNewGame(gamesManager, std::move(game));
    };
  }
}

void CreateGames(Engine::GamesManager& gamesManager)
{
  const Engine::Core::MemDC& screenDC = gamesManager.GetScreen();
  const Size& screenSize = screenDC.GetSize();
  const Size gameSize(screenSize);
  const Point gamePos(0, 0);
  auto game = std::make_unique<Game1>(screenDC, Rect(gamePos, gameSize), RGB(80, 0, 0));
  game->SetEventHandler(Game1::EVENT_EXIT_GAME, CreateQuitAppAction());
  game->SetEventHandler(Game1::EVENT_NEXT_GAME, CreatePush2ndGameAction(gamesManager));

  PushNewGame(gamesManager, std::move(game));

  CreatePush2ndGameAction(gamesManager)();
  CreatePush3rdGameAction(gamesManager)();
  CreatePush4thGameAction(gamesManager)();
}
