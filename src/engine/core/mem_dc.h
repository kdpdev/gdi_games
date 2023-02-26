#pragma once

#include <engine/core/noncopyable.h>
#include <engine/core/primitives.h>

#include <windows.h>

namespace Engine
{
  namespace Core
  {
    class MemDC : private Noncopyable
    {
    public:
      explicit MemDC(HWND hWnd = NULL);
      explicit MemDC(const Size& size, HWND hWnd = NULL);
      ~MemDC();

      HDC GetHDC() const;

      bool IsInited() const;
      Size GetSize() const;
      void Resize(const Size& size);
      void Reset();

    private:
      void Init(const Size& size);

    private:
      HWND    m_hWnd;
      HDC     m_hMemDC;
      HBITMAP m_hNativeBmp;
      HBITMAP m_hBmp;
      Size    m_reservedSize;
      Size    m_size;
    };
  }
}
