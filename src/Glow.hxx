#pragma once

#include <Windows.h>
#include <string>

namespace glow
{
std::string to_string(std::wstring in)
{
    if (!in.empty())
    {
        auto inSize{static_cast<int>(in.size())};

        auto outSize{WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                         in.data(), inSize, nullptr, 0, nullptr, nullptr)};

        if (outSize > 0)
        {
            std::string out;
            out.resize(static_cast<size_t>(outSize));

            if (WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS, in.data(),
                                    inSize, out.data(), outSize, nullptr, nullptr) > 0)
                return out;
        }
    }

    return {};
}

std::wstring to_wide(std::string in)
{

    if (!in.empty())
    {
        auto inSize{static_cast<int>(in.size())};

        auto outSize{
            MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, in.data(), inSize, nullptr, 0)};

        if (outSize > 0)
        {
            std::wstring out;
            out.resize(static_cast<size_t>(outSize));

            if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, in.data(), inSize, out.data(),
                                    outSize) > 0)
                return out;
        }
    }

    return {};
}

std::string bool_to_string(bool in) { return in ? std::string("true") : std::string("false"); }

std::wstring bool_to_wide(bool in) { return in ? std::wstring(L"true") : std::wstring(L"false"); }

void print(std::string in) { OutputDebugStringW(to_wide(in).c_str()); }

void println(std::string in)
{
    OutputDebugStringW(to_wide(in).c_str());
    OutputDebugStringW(L"\n");
}

void wprint(std::wstring in) { OutputDebugStringW(in.c_str()); }

void wprintln(std::wstring in)
{
    OutputDebugStringW(in.c_str());
    OutputDebugStringW(L"\n");
}

void msgbox(std::string in) { MessageBoxW(nullptr, to_wide(in).c_str(), L"Message", 0); };

void wmsgbox(std::wstring in) { MessageBoxW(nullptr, in.c_str(), L"Message", 0); };

int error(std::string in)
{
    std::wstring error = to_wide(in + ". Error: " + std::to_string(GetLastError()));
    MessageBoxW(nullptr, error.c_str(), L"Error", 0);

    return 0;
};

int werror(std::wstring in)
{
    std::wstring error = in + L". Error: " + std::to_wstring(GetLastError());
    MessageBoxW(nullptr, error.c_str(), L"Error", 0);

    return 0;
};
} // namespace glow
