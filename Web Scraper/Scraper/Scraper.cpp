#include "pch.h"
#include "Scraper.h"

namespace Scraper {
	/* static */ bool WebScraper::URLToFile(const String& url, const String& file, BindStatus* bindStatus /* = nullptr */) {
		return SUCCEEDED(::URLDownloadToFile(nullptr, url.c_str(), file.c_str(), 0, bindStatus));
	}

	/* static */ bool WebScraper::URLToFileCache(const String& url, String& file, BindStatus* bindStatus /* = nullptr */) {
		::TCHAR buffer[MAX_PATH];
		auto result = ::URLDownloadToCacheFile(nullptr, url.c_str(), buffer, MAX_PATH, 0, bindStatus);

		file.assign(buffer);
		return SUCCEEDED(result);
	}

	/* static */ void WebScraper::URLToFileAsync(const String& url, const String& file, BindStatus* bindStatus /* = nullptr */, Callback* callback /* = nullptr */) {
		auto _ = async(launch::async, [=, &bindStatus]() {
			auto result = URLToFile(url, file, bindStatus);
		if (callback)
		{
			(*callback)(result);
		}
			});
	}

	/* static */ void WebScraper::URLToFileCacheAsync(const String& url, String& file, BindStatus* bindStatus /* = nullptr */, Callback* callback /* = nullptr */) {
		auto _ = async(launch::async, [&, url]() {
			auto result = URLToFileCache(url, file, bindStatus);
		if (callback)
		{
			(*callback)(result);
		}
			});
	}

	bool WebScraper::URLToString(
		const String& url,
		String& str,
		const InternetStatus* internetStatusOpenURL /* = nullptr */,
		const String& header /* = TEXT("") */,
		const DWORD flagsOpenURL /* = 0 */,
		const DWORD flagsReadFileEx /* = 0 */,
		const DWORD_PTR contextReadFileEx /* = 0 */
	) {
		if (!mHInternetOpen) {
			return false;
		}

		auto hInternetOpenUrl = InternetOpenUrl(
			mHInternetOpen,
			url.c_str(),
			header.empty() ? nullptr : header.c_str(),
			header.empty() ? 0 : -1L,
			flagsOpenURL,
			internetStatusOpenURL ? internetStatusOpenURL->GetContext() : 0
		);
		if (!hInternetOpenUrl) {
			return false;
		}

		if (internetStatusOpenURL &&
			!internetStatusOpenURL->SetInstance(hInternetOpenUrl))
		{
			return false;
		}

		auto buffer = new char[mBufferSize];

		INTERNET_BUFFERS bufferInternet{};
		bufferInternet.dwStructSize = sizeof(INTERNET_BUFFERS);
		bufferInternet.lpvBuffer = buffer;

		BOOL result;
		string strBase;
		do {
			bufferInternet.dwBufferLength = mBufferSize;

			result = InternetReadFileEx(
				hInternetOpenUrl,
				&bufferInternet,
				flagsReadFileEx,
				contextReadFileEx
			);
			if (!result || !bufferInternet.dwBufferLength) {
				break;
			}

			strBase.append(buffer, bufferInternet.dwBufferLength);
		} while (true);
#if defined(_UNICODE) || defined(UNICODE)
		str = ToStringType<wchar_t>(strBase);
#else
		str = move(strBase);
#endif // defined(_UNICODE) || defined(UNICODE)

		delete[] buffer;
		if (internetStatusOpenURL)
		{
			internetStatusOpenURL->ResetInstance(hInternetOpenUrl);
		}
		InternetCloseHandle(hInternetOpenUrl);

		return result;
	}

	void WebScraper::URLToStringAsync(
		const String& url,
		String& str,
		const InternetStatus* internetStatusOpenURL /* = nullptr */,
		Callback* callback /* = nullptr */,
		const String& header /* = TEXT("") */,
		const DWORD flagsOpenURL /* = 0 */,
		const DWORD flagsReadFileEx /* = 0 */,
		const DWORD_PTR contextReadFileEx /* = 0 */
	) {
		auto _ = async(launch::async, [=, &str]() {
			auto result = URLToString(
				url,
				str,
				internetStatusOpenURL,
				header,
				flagsOpenURL,
				flagsReadFileEx,
				contextReadFileEx
			);

		if (callback)
		{
			(*callback)(result);
		}
			});
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
