#include <engine/core/mem_dc.h>
#include <engine/core/err.h>

#include <cassert>

namespace Engine
{
namespace Core
{
  namespace
  {
    class Cleanup : private Noncopyable
    {
    public:
      Cleanup(HDC& hDisplayDC,
              HDC& hMemDC,
              HBITMAP& hNativeBmp,
              HBITMAP& hBmp)
        : m_hDisplayDC(hDisplayDC)
        , m_hMemDC(hMemDC)
        , m_hNativeBmp(hNativeBmp)
        , m_hBmp(hBmp)
        , m_needCleanup(true)
      {
      }

      virtual ~Cleanup()
      {
        ReleaseDC(NULL, m_hDisplayDC);
        m_hDisplayDC = NULL;
        if (m_needCleanup)
        {
          SelectObject(m_hMemDC, m_hNativeBmp);
          DeleteObject(m_hBmp);
          DeleteDC(m_hMemDC);
          m_hMemDC = NULL;
          m_hNativeBmp = NULL;
          m_hBmp = NULL;
        }
      }

      void Cancel()
      {
        m_needCleanup = false;
      }

    private:
      HDC&      m_hDisplayDC;
      HDC&      m_hMemDC;
      HBITMAP&  m_hNativeBmp;
      HBITMAP&  m_hBmp;
      bool      m_needCleanup;
    };

  } // namespace

  MemDC::MemDC(HWND hWnd)
    : m_hWnd(hWnd)
    , m_hMemDC(NULL)
    , m_hNativeBmp(NULL)
    , m_hBmp(NULL)
  {
  }

  MemDC::MemDC(const Size& size, HWND hWnd)
    : m_hWnd(hWnd)
    , m_hMemDC(NULL)
    , m_hNativeBmp(NULL)
    , m_hBmp(NULL)
  {
    Resize(size);
  }

  MemDC::~MemDC()
  {
    Reset();
  }

  HDC MemDC::GetHDC() const
  {
    return m_hMemDC;
  }

  bool MemDC::IsInited() const
  {
    return NULL != m_hMemDC;
  }

  Size MemDC::GetSize() const
  {
    return m_size;
  }

  void MemDC::Resize(const Size& size)
  {
    Init(size);
  }

  void MemDC::Init(const Size& size)
  {
    ERR_THROW_IF(size.width <= 0 || size.height <= 0, "Invalid size.");

    const bool needJustResize = IsInited()
          && m_reservedSize.width >= size.width
          && m_reservedSize.height >= size.height
          ;

    if (needJustResize)
    {
      m_size = size;
      return;
    }

    HDC hDisplayDC = NULL;
    HDC hMemDC = NULL;
    HBITMAP hNativeBmp = NULL;
    HBITMAP hBmp = NULL;

    Cleanup cleanup(hDisplayDC, hMemDC, hNativeBmp, hBmp);

    hDisplayDC = GetDC(m_hWnd);
    ERR_THROW_IF(hDisplayDC == NULL, "GetDC failed.");

    hMemDC = CreateCompatibleDC(hDisplayDC);
    ERR_THROW_IF(hMemDC == NULL, "CreateCompatibleDC failed.");

    hBmp = CreateCompatibleBitmap(hDisplayDC, size.width, size.height);
    ERR_THROW_IF(hBmp == NULL, "CreateCompatibleBitmap failed.");

    hNativeBmp = static_cast<HBITMAP>(SelectObject(hMemDC, hBmp));
    ERR_THROW_IF(hNativeBmp == NULL, "SelectObject failed.");

    cleanup.Cancel();

    Reset();

    m_hMemDC       = hMemDC;
    m_hNativeBmp   = hNativeBmp;
    m_hBmp         = hBmp;
    m_reservedSize = size;
    m_size         = size;
  }

  void MemDC::Reset()
  {
    m_size = Size();
    m_reservedSize = Size();

    //------------------------------------------------------
    // Exceptions must not be generated here
    // becouse the function are called from
    // destructor.
    //------------------------------------------------------

    if (!IsInited())
    {
      return;
    }

    const HBITMAP hBmp = static_cast<HBITMAP>(SelectObject(m_hMemDC, m_hNativeBmp));
    m_hNativeBmp = NULL;
    if (hBmp != m_hBmp)
    {
      assert(!"Native and selected bitmap are not the same.");
      // TODO: do something
    }

    BOOL fSuccess = DeleteObject(m_hBmp);
    m_hBmp = NULL;
    if (!fSuccess)
    {
      assert(!"Failed to delete bitmap.");
      // TODO: do something
    }

    fSuccess = DeleteDC(m_hMemDC);
    m_hMemDC = NULL;
    if (FALSE == fSuccess)
    {
      assert(!"Failed to delete DC");
      // TODO: do something
    }
  }
}
}
