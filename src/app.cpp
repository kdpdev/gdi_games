#include <app.h>
#include <games_factory.h>
#include <main_window.h>

#include <engine/core/action.h>
#include <engine/core/err.h>
#include <engine/core/mem_dc.h>
#include <engine/core/os_primitives.h>
#include <engine/games_manager.h>

#include <chrono>
#include <map>

namespace
{
  LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  int OnError(HWND hWnd, const std::string& errorText);

  const auto GAMES_RENDERING_PERIOD = std::chrono::milliseconds(20);
  const auto GAMES_PROCESSING_PERIOD = std::chrono::milliseconds(10);

  Engine::GamesManager* g_gamesManager = 0;
}

int App::Run(HINSTANCE hInstance)
{
  HWND hWnd = NULL;
  try
  {
    hWnd = CreateMainWindow(hInstance, (WNDPROC)WndProc);
    ERR_THROW_IF(hWnd == NULL, "Failed to create main window.");

    RECT clientRect;
    if (!GetClientRect(hWnd, &clientRect))
    {
      ERR_THROW("Failed to create main window.");
    }

    Engine::GamesManager gamesManager;

    gamesManager.InitScreen(Engine::Core::ToRect(clientRect).size);
    gamesManager.StartProcessingLoop(GAMES_PROCESSING_PERIOD);
    gamesManager.StartRenderingLoop(GAMES_RENDERING_PERIOD);
    gamesManager.SetUpdateScreenAction([hWnd]
    {
      InvalidateRect(hWnd, 0, FALSE);
    });

    g_gamesManager = &gamesManager;

    CreateGames(gamesManager);
    gamesManager.StartTopGame();

    MSG msg;

    while (GetMessageA(&msg, NULL, 0, 0))
    {
      TranslateMessage(&msg);
      DispatchMessageA(&msg);
    }

    return static_cast<int>(msg.wParam);
  }
  catch (...)
  {
    const std::string errorText = Engine::Core::FormatException(std::current_exception());
    return OnError(hWnd, errorText);
  }
}

namespace
{
  bool DoNothing        (HWND hWnd, WPARAM wParam, LPARAM lParam);
  bool OnPaint          (HWND hWnd, WPARAM wParam, LPARAM lParam);
  bool OnDestroy        (HWND hWnd, WPARAM wParam, LPARAM lParam);
  bool OnLeftButtonDown (HWND hWnd, WPARAM wParam, LPARAM lParam);
  bool OnMouseMove      (HWND hWnd, WPARAM wParam, LPARAM lParam);

  typedef bool (*EventHandler)(HWND, WPARAM, LPARAM);
  using EventHandlersMap = std::map<UINT, EventHandler>;

  EventHandlersMap CreateEventHandlersMap()
  {
    EventHandlersMap handlers;
    handlers[WM_DESTROY]     = &OnDestroy;
    handlers[WM_ERASEBKGND]  = &DoNothing;
    handlers[WM_LBUTTONDOWN] = &OnLeftButtonDown;
    handlers[WM_MOUSEMOVE]   = &OnMouseMove;
    handlers[WM_PAINT]       = &OnPaint;
    return handlers;
  }

  const EventHandlersMap& GetEventHandlersMap()
  {
    static EventHandlersMap eventHandlersMap = CreateEventHandlersMap();
    return eventHandlersMap;
  }

  LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    const EventHandlersMap& eventHandlersMap = GetEventHandlersMap();
    const auto handler = eventHandlersMap.find(uMsg);
    if (handler != eventHandlersMap.end())
    {
      const bool isHandled = (handler->second)(hWnd, wParam, lParam);
      if (isHandled)
      {
        return 0;
      }
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
  }

  bool DoNothing(HWND, WPARAM, LPARAM)
  {
    return true;
  }

  bool OnDestroy(HWND, WPARAM, LPARAM)
  {
    PostQuitMessage(0);
    return true;
  }

  bool OnLeftButtonDown(HWND, WPARAM, LPARAM lParam)
  {
    if (g_gamesManager == 0)
    {
      return false;
    }

    const auto xPos = static_cast<Engine::Core::Coord>(LOWORD(lParam));
    const auto yPos = static_cast<Engine::Core::Coord>(HIWORD(lParam));
    return g_gamesManager->OnMouseClick({ xPos, yPos });
  }

  bool OnMouseMove(HWND, WPARAM, LPARAM lParam)
  {
    if (g_gamesManager == 0)
    {
      return false;
    }

    const auto xPos = static_cast<Engine::Core::Coord>(LOWORD(lParam));
    const auto yPos = static_cast<Engine::Core::Coord>(HIWORD(lParam));
    return g_gamesManager->OnMouseMove({ xPos, yPos });
  }

  bool OnPaint(HWND hWnd, WPARAM, LPARAM)
  {
    if (g_gamesManager == 0)
    {
      return false;
    }

    PAINTSTRUCT ps;
    const HDC hDC = BeginPaint(hWnd, &ps);
    const RECT& rect = ps.rcPaint;
    BitBlt(
      hDC,
      rect.left,
      rect.top,
      rect.right - rect.left,
      rect.bottom - rect.top,
      g_gamesManager->GetScreen().GetHDC(),
      rect.left,
      rect.top,
      SRCCOPY);
    EndPaint(hWnd, &ps);

    return true;
  }

  int OnError(HWND hWnd, const std::string& errorText)
  {
    MessageBoxA(hWnd, errorText.c_str(), "Error", MB_ICONHAND | MB_OK);
    if (hWnd != NULL)
    {
      SendMessageA(hWnd, WM_CLOSE, 0, 0);
    }
    return 1;
  }
}
