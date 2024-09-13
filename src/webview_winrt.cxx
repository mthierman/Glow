// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

// #include <glow/webview.hxx>

// #include <glow/log.hxx>

// namespace glow::webview::winrt {
// winrt::IAsyncAction WebViewEnvironment::create(::HWND hwnd) {
//     auto windowRef { winrt::CoreWebView2ControllerWindowReference::CreateFromWindowHandle(
//         glow::math::check_safe_size<uint64_t>(reinterpret_cast<uintptr_t>(hwnd))) };

//     auto environmentOptions { winrt::CoreWebView2EnvironmentOptions() };

//     environment = co_await winrt::CoreWebView2Environment::CreateWithOptionsAsync(
//         L"", L"", environmentOptions);

//     controller = co_await environment.CreateCoreWebView2ControllerAsync(windowRef);

//     auto color { winrt::Windows::UI::Colors::Black() };
//     controller.DefaultBackgroundColor(color);

//     core = controller.CoreWebView2();

//     auto clientRect { glow::window::get_client_rect(hwnd) };

//     winrt::Windows::Foundation::Rect rect;
//     rect.Height = static_cast<float>(clientRect.bottom);
//     rect.Width = static_cast<float>(clientRect.right);
//     rect.X = 0;
//     rect.Y = 0;

//     core.Navigate(L"https://www.google.ca");

//     controller.Bounds(rect);

//     core.SourceChanged([](const winrt::CoreWebView2& /* sender */,
//                           const winrt::CoreWebView2SourceChangedEventArgs& args) {
//         glow::log::log("SourceChanged: {}", args.IsNewDocument());
//     });

//     core.NavigationCompleted(
//         [](const winrt::CoreWebView2& /* sender */,
//            const winrt::CoreWebView2NavigationCompletedEventArgs& /* args */) {});
// }
// }; // namespace glow::webview::winrt
