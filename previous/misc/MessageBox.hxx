#pragma once

class MsgBox
{
  public:
    MsgBox();
    ~MsgBox();

  private:
    static __int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
    int wm_close(HWND, UINT, WPARAM, LPARAM);
    int wm_destroy(HWND, UINT, WPARAM, LPARAM);
    int wm_erasebkgnd(HWND, UINT, WPARAM, LPARAM);
    int wm_getminmaxinfo(HWND, UINT, WPARAM, LPARAM);
};
