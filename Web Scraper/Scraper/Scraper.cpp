#include "pch.h"
#include "Scraper.h"

namespace Scraper {
    HRESULT URLToFile(const String& url, const String& file, DownloadProgress* downloadProgress /* = nullptr */) {
        return URLDownloadToFile(nullptr, url.c_str(), file.c_str(), 0, downloadProgress);
    }

    HRESULT URLToFileCache(const String& url, String& file, DownloadProgress* downloadProgress /* = nullptr */) {
        ::TCHAR buffer[MAX_PATH];
        auto result = URLDownloadToCacheFile(nullptr, url.c_str(), buffer, MAX_PATH, 0, downloadProgress);

        file.assign(buffer);
        return result;
    }

    future<HRESULT> URLToFileAsync(const String& url, const String& file, DownloadProgress* downloadProgress /* = nullptr */) {
        return async(launch::async, [=, &downloadProgress] () { return URLToFile(url, file, downloadProgress); });
    }

    future<HRESULT> URLToFileCacheAsync(const String& url, String& file, DownloadProgress* downloadProgress /* = nullptr */) {
        return async(launch::async, [&, url] () { return URLToFileCache(url, file, downloadProgress); });
    }
}
