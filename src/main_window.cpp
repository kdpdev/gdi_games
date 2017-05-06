#include "main_window.h"
#include <cassert>

namespace
{
  const char* const APP_NAME = "GDI Games";

  WNDCLASSA GetWindowClass(HINSTANCE hInstance, WNDPROC winProc);
  SIZE GetScreenSize();
  SIZE GetWindowSize();
  POINT GetWindowPosition();
  DWORD GetWindowStyle();
}

HWND CreateMainWindow(HINSTANCE hInstance, WNDPROC winProc)
{
  const WNDCLASSA winClass = GetWindowClass(hInstance, winProc);

  const ATOM registeredWinId = RegisterClassA(&winClass);
  if (registeredWinId == 0)
  {
    return NULL;
  }

  const SIZE winSize = GetWindowSize();
  const POINT winPos = GetWindowPosition();

  const HWND hWnd = CreateWindowA(
    APP_NAME,
    APP_NAME,
    GetWindowStyle(),
    winPos.x,
    winPos.y,
    winSize.cx,
    winSize.cy,
    NULL,
    NULL,
    hInstance,
    NULL);

  return hWnd;
}

namespace
{
  WNDCLASSA GetWindowClass(HINSTANCE hInstance, WNDPROC winProc)
  {
    assert(winProc != 0 && "Window procedure is null.");

    WNDCLASSA winClass;
    winClass.style = CS_HREDRAW | CS_VREDRAW;
    winClass.lpfnWndProc = winProc;
    winClass.cbClsExtra = 0;
    winClass.cbWndExtra = 0;
    winClass.hInstance = hInstance;
    winClass.hIcon = NULL;
    winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    winClass.lpszMenuName = 0;
    winClass.lpszClassName = APP_NAME;

    return winClass;
  }

  SIZE GetScreenSize()
  {
    const SIZE size =
    {
      GetSystemMetrics(SM_CXSCREEN),
      GetSystemMetrics(SM_CYSCREEN)
    };
    return size;
  }

  SIZE GetWindowSize()
  {
    const SIZE screenSize = GetScreenSize();
    const LONG wndSide = min(screenSize.cx / 2, screenSize.cy / 2);
    const SIZE wndSize = {wndSide, wndSide};
    return wndSize;
  }

  POINT GetWindowPosition()
  {
    const SIZE screenSize = GetScreenSize();
    const SIZE wndSize = GetWindowSize();
    const POINT wndPos =
    {
      (screenSize.cx - wndSize.cx) / 2,
      (screenSize.cy - wndSize.cy) / 2
    };
    return wndPos;
  }

  DWORD GetWindowStyle()
  {
    return WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
    //return WS_VISIBLE | WS_POPUP | WS_BORDER;
  }
}
