#pragma once

#include "Shared/String.h"

class BindStatus;

namespace Scraper {
    using namespace Shared;

    constexpr auto DEFAULT_USER_AGENT = TEXT("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36");
    constexpr auto DEFAULT_BUFFER_SIZE = 8192;

    bool URLToFile(const String& url, const String& file, BindStatus* bindStatus = nullptr);
    future<bool> URLToFileAsync(const String& url, const String& file, BindStatus* bindStatus = nullptr);

    bool URLToFileCache(const String& url, String& file, BindStatus* bindStatus = nullptr);
    future<bool> URLToFileCacheAsync(const String& url, String& file, BindStatus* bindStatus = nullptr);

    template <class _Elem = char, class _Traits = char_traits<_Elem>, class _Alloc = allocator<_Elem>>
    inline bool URLToString(
        const String& url,
        basic_string<_Elem, _Traits, _Alloc>& str,
        const String& userAgent = DEFAULT_USER_AGENT,
        const DWORD accessType = INTERNET_OPEN_TYPE_PRECONFIG,
        const String& proxy = TEXT(""),
        const String& proxyBypass = TEXT(""),
        const DWORD flagsOpen = 0,
        const String& header = TEXT(""),
        const DWORD headerLength = 0,
        const DWORD flagsOpenURL = 0,
        const DWORD_PTR contextOpenURL = 0,
        const DWORD bufferSize = DEFAULT_BUFFER_SIZE,
        const DWORD flagsReadFileEx = 0,
        const DWORD_PTR contextReadFileEx = 0
    ) {
        auto hInternetOpen = InternetOpen(
            userAgent.c_str(),
            accessType,
            proxy.empty() ? nullptr : proxy.c_str(),
            proxyBypass.empty() ? nullptr : proxyBypass.c_str(),
            flagsOpen
        );
        if (!hInternetOpen) {
            return false;
        }

        auto hInternetOpenUrl = InternetOpenUrl(
            hInternetOpen,
            url.c_str(),
            header.empty() ? nullptr : header.c_str(),
            headerLength,
            flagsOpenURL,
            contextOpenURL
        );
        if (!hInternetOpenUrl) {
            InternetCloseHandle(hInternetOpen);
            return false;
        }

        auto buffer = new _Elem[bufferSize];

        INTERNET_BUFFERS bufferInternet {};
        bufferInternet.dwStructSize = sizeof(INTERNET_BUFFERS);
        bufferInternet.lpvBuffer = buffer;

        BOOL result;
        do {
            bufferInternet.dwBufferLength = bufferSize;

            result = InternetReadFileEx(
                hInternetOpenUrl,
                &bufferInternet,
                flagsReadFileEx,
                contextReadFileEx
            );
            if (!result || !bufferInternet.dwBufferLength) {
                break;
            }

            str.append(buffer, bufferInternet.dwBufferLength);
        } while (true);

        delete[] buffer;
        InternetCloseHandle(hInternetOpenUrl);

        return result;
    }

    template <class _Elem = char, class _Traits = char_traits<_Elem>, class _Alloc = allocator<_Elem>>
    inline future<bool> URLToStringAsync(
        const String& url,
        basic_string<_Elem, _Traits, _Alloc>& str,
        const String& userAgent = DEFAULT_USER_AGENT,
        const DWORD accessType = INTERNET_OPEN_TYPE_PRECONFIG,
        const String& proxy = TEXT(""),
        const String& proxyBypass = TEXT(""),
        const DWORD flagsOpen = 0,
        const String& header = TEXT(""),
        const DWORD headerLength = 0,
        const DWORD flagsOpenURL = 0,
        const DWORD_PTR contextOpenURL = 0,
        const DWORD bufferSize = DEFAULT_BUFFER_SIZE,
        const DWORD flagsReadFileEx = 0,
        const DWORD_PTR contextReadFileEx = 0
    ) {
        return async(launch::async, [=, &str] () {
            return URLToString(
                url,
                str,
                userAgent,
                accessType,
                proxy,
                proxyBypass,
                flagsOpen,
                header,
                headerLength,
                flagsOpenURL,
                contextOpenURL,
                bufferSize,
                flagsReadFileEx,
                contextReadFileEx
            );
                     });
    }
}
