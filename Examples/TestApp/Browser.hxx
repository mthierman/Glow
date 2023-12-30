// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <gui/webview.hxx>

using namespace glow::gui;

struct CoreWebView2
{
    auto create_environment(HWND hwnd) -> void;
    auto create_controller(HWND hwnd, ICoreWebView2Environment* environment) -> void;

    wil::com_ptr<ICoreWebView2EnvironmentOptions6> m_evironmentOptions6{nullptr};
    wil::com_ptr<ICoreWebView2Controller> m_controller{nullptr};
    wil::com_ptr<ICoreWebView2Controller4> m_controller4{nullptr};
    wil::com_ptr<ICoreWebView2> m_core{nullptr};
    wil::com_ptr<ICoreWebView2_20> m_core20{nullptr};
    wil::com_ptr<ICoreWebView2Settings> m_settings{nullptr};
    wil::com_ptr<ICoreWebView2Settings8> m_settings8{nullptr};

    wil::unique_hcursor m_hCursor{static_cast<HCURSOR>(
        LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    wil::unique_hicon m_hIcon{static_cast<HICON>(
        LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    wil::unique_hbrush m_hbrBackground{static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))};
};

auto CoreWebView2::create_environment(HWND hwnd) -> void
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");

    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [=, this](HRESULT errorCode, ICoreWebView2Environment* createdEnvironment) -> HRESULT
            {
                if (createdEnvironment) create_controller(hwnd, createdEnvironment);

                return S_OK;
            })
            .Get());
}

auto CoreWebView2::create_controller(HWND hwnd, ICoreWebView2Environment* environment) -> void
{
    environment->CreateCoreWebView2Controller(
        hwnd, Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                  [=, this](HRESULT, ICoreWebView2Controller* controller) -> HRESULT
                  {
                      if (controller)
                      {
                          m_controller = controller;
                          m_controller4 = m_controller.try_query<ICoreWebView2Controller4>();

                          COREWEBVIEW2_COLOR bgColor{0, 0, 0, 0};
                          m_controller4->put_DefaultBackgroundColor(bgColor);

                          m_controller->get_CoreWebView2(m_core.put());
                          m_core20 = m_core.try_query<ICoreWebView2_20>();

                          m_core20->Navigate(L"https://localhost:8000/tabs/index.html");
                      }

                      return S_OK;
                  })
                  .Get());
}

struct Browser final : public WebView2
{
    using WebView2::WebView2;

    CoreWebView2 gui;

  private:
    auto on_size() -> int override;
};

auto Browser::on_size() -> int
{
    RECT rect{};
    GetClientRect(m_hwnd.get(), &rect);

    if (m_controller4)
    {
        m_controller4->put_Bounds(RECT{0, 0, rect.right - rect.left, rect.bottom / 2});
        // m_controller4->put_Bounds(
        //     RECT{0, (rect.bottom - rect.top) / 2, rect.right - rect.left, rect.bottom});
    }

    if (gui.m_controller4)
    {
        gui.m_controller4->put_Bounds(
            RECT{0, (rect.bottom - rect.top) / 2, rect.right - rect.left, rect.bottom});
        // gui.m_controller4->put_Bounds(RECT{0, 0, rect.right - rect.left, rect.bottom / 2});
    }

    return 0;
}
