#include "pch.h"
#include "Scraper.h"
#include "Progress/BindStatus.h"

namespace Scraper {
    using namespace Progress;

    bool URLToFile(const String& url, const String& file, BindStatus* bindStatus /* = nullptr */) {
        return SUCCEEDED(::URLDownloadToFile(nullptr, url.c_str(), file.c_str(), 0, bindStatus));
    }

    bool URLToFileCache(const String& url, String& file, BindStatus* bindStatus /* = nullptr */) {
        ::TCHAR buffer[MAX_PATH];
        auto result = ::URLDownloadToCacheFile(nullptr, url.c_str(), buffer, MAX_PATH, 0, bindStatus);

        file.assign(buffer);
        return SUCCEEDED(result);
    }

    future<bool> URLToFileAsync(const String& url, const String& file, BindStatus* bindStatus /* = nullptr */) {
        return async(launch::async, [=, &bindStatus] () { return URLToFile(url, file, bindStatus); });
    }

    future<bool> URLToFileCacheAsync(const String& url, String& file, BindStatus* bindStatus /* = nullptr */) {
        return async(launch::async, [&, url] () { return URLToFileCache(url, file, bindStatus); });
    }
}
