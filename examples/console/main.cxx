#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gdiplus.h>
#include <gdipluscolor.h>
#include <gdipluspixelformats.h>
#endif

#include <stdlib.h>

#include <print>

#include <glow/glow.hxx>

auto main() -> int {
    glow::system::GdiPlus gdiPlus;

    auto color { Gdiplus::Color() };
    color.SetFromCOLORREF(RGB(255, 0, 0));

    std::println("{}", color.GetR());

    return EXIT_SUCCESS;
}
