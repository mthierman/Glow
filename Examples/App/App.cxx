// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <config/glow.hxx>

#include <gui/mainwindow.hxx>
#include <gui/webview.hxx>
#include <logging/logging.hxx>

struct WebView2 final : public glow::gui::WebView2
{
    using glow::gui::WebView2::WebView2;

    auto initialized() -> void override;
};

auto WebView2::initialized() -> void
{
    OutputDebugStringA("Initialized!");

    // m_settings8->put_AreBrowserAcceleratorKeysEnabled(true);
    m_settings8->put_AreDefaultContextMenusEnabled(false);
    // m_settings8->put_AreDefaultScriptDialogsEnabled(true);
    // m_settings8->put_AreDevToolsEnabled(true);
    // m_settings8->put_AreHostObjectsAllowed(true);
    // m_settings8->put_HiddenPdfToolbarItems(
    //     COREWEBVIEW2_PDF_TOOLBAR_ITEMS::COREWEBVIEW2_PDF_TOOLBAR_ITEMS_NONE);
    // m_settings8->put_IsBuiltInErrorPageEnabled(true);
    // m_settings8->put_IsGeneralAutofillEnabled(true);
    // m_settings8->put_IsPasswordAutosaveEnabled(true);
    // m_settings8->put_IsPinchZoomEnabled(true);
    // m_settings8->put_IsReputationCheckingRequired(true);
    // m_settings8->put_IsScriptEnabled(true);
    // m_settings8->put_IsStatusBarEnabled(true);
    // m_settings8->put_IsSwipeNavigationEnabled(true);
    // m_settings8->put_IsWebMessageEnabled(true);
    // m_settings8->put_IsZoomControlEnabled(true);
}

struct App final : public glow::gui::MainWindow
{
    using glow::gui::MainWindow::MainWindow;

    static auto CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_parent_notify(WPARAM wParam) -> int;
    auto on_size() -> int;
};

auto CALLBACK App::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto gwlId{GetWindowLongPtrA(hWnd, GWL_ID)};

    auto rect{*std::bit_cast<LPRECT>(lParam)};
    auto position{glow::gui::rect_to_position(rect)};

    if (gwlId == 1)
        SetWindowPos(hWnd, nullptr, 0, 0, position.width, position.height, SWP_NOZORDER);

    return TRUE;
}

auto App::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_PARENTNOTIFY: return on_parent_notify(wParam);
    case WM_SIZE: return on_size();
    }

    return DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto App::on_parent_notify(WPARAM wParam) -> int
{
    if (LOWORD(wParam) == WM_CREATE) on_size();

    return 0;
}

auto App::on_size() -> int
{
    RECT rect{0};
    GetClientRect(m_hwnd.get(), &rect);
    EnumChildWindows(m_hwnd.get(), EnumChildProc, std::bit_cast<LPARAM>(&rect));
    Sleep(1);

    return 0;
}

auto main() -> int
{
    glow::gui::CoInitialize init;

    if (SUCCEEDED(init))
    {
        App app;
        WebView2 webView{app.m_hwnd.get(), 1};

        app.show_normal();
        webView.show_normal();

        glow::gui::message_loop();
    }

    return 0;
}
