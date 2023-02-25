#include "pch.h"
#include "Scraper.h"
#include "Progress/BindStatus.h"

namespace Scraper {
	using namespace Progress;

	/* static */ bool URLToFile(const String& url, const String& file, BindStatus* bindStatus /* = nullptr */) {
		return SUCCEEDED(::URLDownloadToFile(nullptr, url.c_str(), file.c_str(), 0, bindStatus));
	}

	/* static */ bool URLToFileCache(const String& url, String& file, BindStatus* bindStatus /* = nullptr */) {
		::TCHAR buffer[MAX_PATH];
		auto result = ::URLDownloadToCacheFile(nullptr, url.c_str(), buffer, MAX_PATH, 0, bindStatus);

		file.assign(buffer);
		return SUCCEEDED(result);
	}

	/* static */ future<bool> URLToFileAsync(const String& url, const String& file, BindStatus* bindStatus /* = nullptr */) {
		return async(launch::async, [=, &bindStatus]() { return URLToFile(url, file, bindStatus); });
	}

	/* static */ future<bool> URLToFileCacheAsync(const String& url, String& file, BindStatus* bindStatus /* = nullptr */) {
		return async(launch::async, [&, url]() { return URLToFileCache(url, file, bindStatus); });
	}

	WebScraper::WebScraper(
		const String& userAgent /* = DEFAULT_USER_AGENT */,
		const DWORD accessType /* = INTERNET_OPEN_TYPE_PRECONFIG */,
		const String& proxy /* = TEXT("") */,
		const String& proxyBypass /* = TEXT("") */,
		const DWORD flagsOpen /* = 0 */
	) {
		mHInternetOpen = InternetOpen(
			userAgent.c_str(),
			accessType,
			proxy.empty() ? nullptr : proxy.c_str(),
			proxyBypass.empty() ? nullptr : proxyBypass.c_str(),
			flagsOpen
		);
	}

	void WebScraper::SetBufferSize(const DWORD bufferSize) {
		mBufferSize = bufferSize;
	}

	DWORD WebScraper::GetBufferSize() const {
		return mBufferSize;
	}
}
