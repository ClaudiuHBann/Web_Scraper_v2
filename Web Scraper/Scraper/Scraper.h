#pragma once

#include "Shared/String.h"
#include "Progress/BindStatus.h"
#include "Progress/InternetStatus.h"

namespace Scraper {
	using namespace Progress;

	constexpr auto DEFAULT_USER_AGENT = TEXT("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36");
	constexpr auto DEFAULT_BUFFER_SIZE = 8192;

	class WebScraper
	{
	public:
		using Callback = function<void(bool)>;

		WebScraper(
			const String& userAgent = DEFAULT_USER_AGENT,
			const DWORD accessType = INTERNET_OPEN_TYPE_PRECONFIG,
			const String& proxy = TEXT(""),
			const String& proxyBypass = TEXT(""),
			const DWORD flagsOpen = 0
		);

		void SetBufferSize(const DWORD bufferSize);

		DWORD GetBufferSize() const;

		static bool URLToFile(const String& url, const String& file, BindStatus* bindStatus = nullptr);
		static void URLToFileAsync(const String& url, const String& file, BindStatus* bindStatus = nullptr, Callback* callback = nullptr);

		static bool URLToFileCache(const String& url, String& file, BindStatus* bindStatus = nullptr);
		static void URLToFileCacheAsync(const String& url, String& file, BindStatus* bindStatus = nullptr, Callback* callback = nullptr);

		bool URLToString(
			const String& url,
			String& str,
			const InternetStatus* internetStatusOpenURL = nullptr,
			const String& header = TEXT(""),
			const DWORD flagsOpenURL = 0,
			const DWORD flagsReadFileEx = 0,
			const DWORD_PTR contextReadFileEx = 0
		);

		void URLToStringAsync(
			const String& url,
			String& str,
			const InternetStatus* internetStatusOpenURL = nullptr,
			Callback* callback = nullptr,
			const String& header = TEXT(""),
			const DWORD flagsOpenURL = 0,
			const DWORD flagsReadFileEx = 0,
			const DWORD_PTR contextReadFileEx = 0
		);

		~WebScraper() {
			if (mHInternetOpen)
			{
				InternetCloseHandle(mHInternetOpen);
			}
		}

	private:
		HINTERNET mHInternetOpen{};
		atomic<DWORD> mBufferSize = DEFAULT_BUFFER_SIZE;
	};
}
