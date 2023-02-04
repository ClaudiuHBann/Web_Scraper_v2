#pragma once

#include "Shared/String.h"
#include "DownloadProgress.h"

using namespace Shared;

namespace Scraper {
    HRESULT URLToFile(const String& url, const String& file, DownloadProgress* downloadProgress = nullptr);
    future<HRESULT> URLToFileAsync(const String& url, const String& file, DownloadProgress* downloadProgress = nullptr);
    HRESULT URLToFileCache(const String& url, String& file, DownloadProgress* downloadProgress = nullptr);
    future<HRESULT> URLToFileCacheAsync(const String& url, String& file, DownloadProgress* downloadProgress = nullptr);
}
