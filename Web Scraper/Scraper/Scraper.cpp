#include "pch.h"
#include "Scraper.h"
#include "DownloadProgress.h"

namespace Scraper {
    bool URLToFile(const String& url, const String& file, DownloadProgress* downloadProgress /* = nullptr */) {
        return SUCCEEDED(::URLDownloadToFile(nullptr, url.c_str(), file.c_str(), 0, downloadProgress));
    }

    bool URLToFileCache(const String& url, String& file, DownloadProgress* downloadProgress /* = nullptr */) {
        ::TCHAR buffer[MAX_PATH];
        auto result = ::URLDownloadToCacheFile(nullptr, url.c_str(), buffer, MAX_PATH, 0, downloadProgress);

        file.assign(buffer);
        return SUCCEEDED(result);
    }

    future<bool> URLToFileAsync(const String& url, const String& file, DownloadProgress* downloadProgress /* = nullptr */) {
        return async(launch::async, [=, &downloadProgress] () { return URLToFile(url, file, downloadProgress); });
    }

    future<bool> URLToFileCacheAsync(const String& url, String& file, DownloadProgress* downloadProgress /* = nullptr */) {
        return async(launch::async, [&, url] () { return URLToFileCache(url, file, downloadProgress); });
    }
}
