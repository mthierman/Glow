// clang-format off
// ╔──────────────────────╗
// │                      │
// │     ╔═╗╦  ╔═╗╦ ╦     │  Glow - https://github.com/mthierman/Glow
// │     ║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                      │
// ╚──────────────────────╝
// clang-format on

#ifndef GDIPVER
#define GDIPVER 0x0110
#endif

#include <Windows.h>
#include <gdiplus.h>
#include <wil/com.h>
#include <wil/resource.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>
#include <print>
#include <utility>
#include <vector>

#include <console/console.hxx>
#include <text/text.hxx>

auto get_encoder_clsid(const std::wstring& format, CLSID* pClsid) -> bool;
auto get_bitmap(const std::filesystem::path& inputCanonical, const int& size, CLSID* pClsid)
    -> std::vector<char>;
auto write_header(std::ofstream& outputStream, uint16_t count) -> void;
auto write_entry(std::ofstream& outputStream, std::vector<char>& bitmap, uint8_t size,
                 uint32_t offset) -> void;
auto write_bitmap(std::ofstream& outputStream, std::vector<char>& bitmap) -> void;

auto main() -> int
{
    ULONG_PTR gdiplusToken;
    auto gdiplusStartupInput{std::make_unique<Gdiplus::GdiplusStartupInput>()};

    if (Gdiplus::GdiplusStartup(&gdiplusToken, gdiplusStartupInput.get(), nullptr) !=
        Gdiplus::Status::Ok)
        return 0;

    auto argv{glow::console::get_argv()};

    if (argv.size() == 3)
    {
        std::filesystem::path inputFile{argv[1]};
        std::filesystem::path outputFile{argv[2]};
        std::filesystem::path inputCanonical;

        if (!std::filesystem::exists(inputFile)) return 0;

        try
        {
            inputCanonical = std::filesystem::canonical(inputFile);
        }
        catch (std::filesystem::filesystem_error& /*e*/)
        {
            return 0;
        }

        if (inputCanonical.empty()) return 0;

        auto pngClsid{std::make_unique<CLSID>()};

        if (!get_encoder_clsid(L"image/png", pngClsid.get())) return 0;

        std::vector<std::vector<char>> bitmaps;
        std::vector<int> bitmapSizes{256, 128, 96, 80, 72, 64, 60, 48, 40, 36, 32, 30, 24, 20, 16};

        for (auto i = 0; i < bitmapSizes.size(); i++)
        {
            bitmaps.push_back(
                get_bitmap(inputCanonical.wstring().c_str(), bitmapSizes[i], pngClsid.get()));
        }

        std::vector<uint32_t> sizes;
        sizes.reserve(bitmaps.size());
        for (auto const& bitmap : bitmaps)
        {
            sizes.push_back(static_cast<uint32_t>(bitmap.size()));
        }

        std::ofstream outputStream;
        uint16_t count{static_cast<uint16_t>(bitmapSizes.size())};
        uint32_t offset{6 + (16 * static_cast<uint32_t>(count))};

        std::vector<uint32_t> positions;
        positions.push_back(offset);
        auto cumulate{offset};
        for (auto i = 0; i < sizes.size(); ++i)
        {
            cumulate += sizes[i];
            positions.push_back(cumulate);
        }

        std::vector<uint8_t> dimensions;
        dimensions.reserve(sizes.size());
        for (const auto& size : sizes)
        {
            dimensions.push_back(static_cast<uint8_t>(size));
        }

        outputStream.open(outputFile, std::ios::binary);

        write_header(outputStream, count);

        write_entry(outputStream, bitmaps[0], 0, positions[0]);
        for (int i = 1; i < static_cast<int>(bitmapSizes.size()); i++)
        {
            write_entry(outputStream, bitmaps[i], dimensions[i], positions[i]);
        }

        for (int i = 0; i < static_cast<int>(bitmapSizes.size()); i++)
        {
            write_bitmap(outputStream, bitmaps[i]);
        }
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;
}

auto get_encoder_clsid(const std::wstring& format, CLSID* pClsid) -> bool
{
    UINT num{0};
    UINT size{0};

    if (Gdiplus::GetImageEncodersSize(&num, &size) != Gdiplus::Status::Ok) return false;

    std::vector<Gdiplus::ImageCodecInfo> codecs(size);

    if (Gdiplus::GetImageEncoders(num, size, codecs.data()) != Gdiplus::Status::Ok) return false;

    for (auto codec : codecs)
    {
        if (format == std::wstring{codec.MimeType})
        {
            *pClsid = codec.Clsid;
            return true;
        }
    }

    return false;
}

auto get_bitmap(const std::filesystem::path& inputCanonical, const int& size, CLSID* pClsid)
    -> std::vector<char>
{
    auto src{std::unique_ptr<Gdiplus::Image>(
        Gdiplus::Image::FromFile(inputCanonical.wstring().c_str()))};
    auto dst{std::make_unique<Gdiplus::Bitmap>(size, size)};
    auto g{std::unique_ptr<Gdiplus::Graphics>(Gdiplus::Graphics::FromImage(dst.get()))};

    // https://photosauce.net/blog/post/image-scaling-with-gdi-part-3-drawimage-and-the-settings-that-affect-it
    g->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias8x8);
    g->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    g->SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
    g->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
    g->SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);

    auto attr{std::make_unique<Gdiplus::ImageAttributes>()};
    attr->SetWrapMode(Gdiplus::WrapModeTileFlipXY);
    auto rect{Gdiplus::Rect(0, 0, size, size)};

    g->DrawImage(src.get(), rect, 0, 0, src->GetWidth(), src->GetHeight(), Gdiplus::Unit::UnitPixel,
                 attr.get(), 0, 0);

    wil::unique_hglobal hglobal;
    wil::com_ptr<IStream> istream;

    if (FAILED(::CreateStreamOnHGlobal(hglobal.get(), TRUE, &istream))) return {};

    dst->Save(istream.get(), pClsid);

    if (FAILED(::GetHGlobalFromStream(istream.get(), &hglobal))) return {};

    auto bufsize{::GlobalSize(hglobal.get())};
    auto* ptr{::GlobalLock(hglobal.get())};

    if (ptr == nullptr) return {};

    std::vector<char> vec;
    vec.resize(bufsize);

    std::copy(static_cast<char*>(ptr), (static_cast<char*>(ptr) + bufsize), vec.begin());

    ::GlobalUnlock(hglobal.get());

    return vec;
}

auto write_header(std::ofstream& outputStream, uint16_t count) -> void
{
    uint16_t reserved{0};
    uint16_t type{1};

    // Header
    // 0-1 Reserved, Must always be 0.
    outputStream.write(reinterpret_cast<char*>(&reserved), sizeof(reserved));
    // 2-3 Image type, 1 = icon (.ICO), 2 = cursor (.CUR).
    outputStream.write(reinterpret_cast<char*>(&type), sizeof(type));
    // 4-5 Number of images.
    outputStream.write(reinterpret_cast<char*>(&count), sizeof(count));
}

auto write_entry(std::ofstream& outputStream, std::vector<char>& bitmap, uint8_t size,
                 uint32_t offset) -> void
{
    uint8_t reserved{0};
    uint8_t colors{0};
    uint16_t planes{1};
    uint16_t bits{32};
    auto bitmapSize{static_cast<uint32_t>(bitmap.size())};

    // Entry
    // 0 Image width in pixels. Range is 0-255. 0 means 256 pixels.
    outputStream.write(reinterpret_cast<char*>(&size), sizeof(size));
    // 1 Image height in pixels. Range is 0-255. 0 means 256 pixels.
    outputStream.write(reinterpret_cast<char*>(&size), sizeof(size));
    // 2 Number of colors in the color palette. Should be 0 if no palette.
    outputStream.write(reinterpret_cast<char*>(&colors), sizeof(colors));
    // 3 Reserved. Should be 0.
    outputStream.write(reinterpret_cast<char*>(&reserved), sizeof(reserved));
    // 4-5
    // .ICO: Color planes (0 or 1).
    // .CUR: Horizontal coordinates of the hotspot in pixels from the left.
    outputStream.write(reinterpret_cast<char*>(&planes), sizeof(planes));
    // 6-7
    // .ICO: Bits per pixel.
    // .CUR: Vertical coordinates of the hotspot in pixels from the top.
    outputStream.write(reinterpret_cast<char*>(&bits), sizeof(bits));
    // 8-11 Size of the image's data in bytes
    outputStream.write(reinterpret_cast<char*>(&bitmapSize), sizeof(bitmapSize));
    // 12-15 Offset of image data from the beginning of the file.
    outputStream.write(reinterpret_cast<char*>(&offset), sizeof(offset));
}

auto write_bitmap(std::ofstream& outputStream, std::vector<char>& bitmap) -> void
{
    // Write image data. PNG must be stored in its entirety, with file header & must
    // be 32bpp ARGB format.
    auto bitmapSize{static_cast<int64_t>(bitmap.size())};
    auto charSize{static_cast<int64_t>(sizeof(char))};
    outputStream.write(reinterpret_cast<char*>(bitmap.data()), (bitmapSize * charSize));
}
