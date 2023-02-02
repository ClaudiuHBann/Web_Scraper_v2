#pragma once

#include "Shared/String.h"

using namespace Shared;

class DownloadProgress;

namespace Scraper {
    HRESULT URLToFile(const String& url, const String& file, DownloadProgress* downloadProgress = nullptr);
    future<HRESULT> URLToFileAsync(const String& url, const String& file, DownloadProgress* downloadProgress = nullptr);
    HRESULT URLToFileCache(const String& url, String& file, DownloadProgress* downloadProgress = nullptr);
    future<HRESULT> URLToFileCacheAsync(const String& url, String& file, DownloadProgress* downloadProgress = nullptr);
}
