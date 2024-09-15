// clang-format off
// Glow - https://github.com/mthierman/Glow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include <glow/color.hxx>

#include <glow/text.hxx>

namespace glow::color {
Color::Color(uint8_t r, uint8_t g, uint8_t b)
    : r { r },
      g { g },
      b { b },
      a { 255 } { }

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r { r },
      g { g },
      b { b },
      a { a } { }

Color::Color(winrt::UIColorType colorType) {
    auto color { uiSettings.GetColorValue(colorType) };

    r = color.R;
    g = color.G;
    b = color.B;
    a = color.A;
}

Color::Color(winrt::UIElementType elementType) {
    auto color { uiSettings.UIElementColor(elementType) };

    r = color.R;
    g = color.G;
    b = color.B;
    a = color.A;
}

Color::Color(const winrt::Color& color)
    : r { color.R },
      g { color.G },
      b { color.B },
      a { color.A } { }

Color::Color(const ::COLORREF& colorRef)
    : r { GetRValue(colorRef) },
      g { GetGValue(colorRef) },
      b { GetBValue(colorRef) },
      a { 255 } { }

Color::Color(const COREWEBVIEW2_COLOR& coreWebView2Color)
    : r { coreWebView2Color.R },
      g { coreWebView2Color.G },
      b { coreWebView2Color.B },
      a { coreWebView2Color.A } { }

auto Color::brush() const -> ::HBRUSH { return ::CreateSolidBrush(RGB(r, g, b)); }

auto Color::hex() const -> std::string {
    return std::format("#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", r, g, b, a);
}

auto Color::colorref() const -> ::COLORREF { return RGB(r, g, b); }

auto Color::winrt_color() const -> winrt::Color {
    return winrt::Color { .A { a }, .R { r }, .G { g }, .B { b } };
}

auto Color::webview2_color() const -> COREWEBVIEW2_COLOR {
    return COREWEBVIEW2_COLOR { .A { a }, .R { r }, .G { g }, .B { b } };
}

auto Color::is_dark() const -> bool { return (((5 * g) + (2 * r) + b) < (8 * 128)) ? true : false; }

auto Color::string() const -> std::string {
    return std::format("R: {} G: {} B: {} A: {}", r, g, b, a);
}

auto Color::wstring() const -> std::wstring { return glow::text::to_wstring(string()); }
}; // namespace glow::color
