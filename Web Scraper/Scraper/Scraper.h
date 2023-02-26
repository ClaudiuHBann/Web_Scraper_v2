#pragma once

#include "Shared/String.h"
#include "Progress/InternetStatus.h"

class BindStatus;

namespace Scraper {
	using namespace Shared;
	using namespace Progress;

	constexpr auto DEFAULT_USER_AGENT = TEXT("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36");
	constexpr auto DEFAULT_BUFFER_SIZE = 8192;

	class WebScraper
	{
	public:
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
		static future<bool> URLToFileAsync(const String& url, const String& file, BindStatus* bindStatus = nullptr);

		static bool URLToFileCache(const String& url, String& file, BindStatus* bindStatus = nullptr);
		static future<bool> URLToFileCacheAsync(const String& url, String& file, BindStatus* bindStatus = nullptr);

		// FIXME I think this implementation doesn't support async internet operations
		template <class _Elem = char, class _Traits = char_traits<_Elem>, class _Alloc = allocator<_Elem>>
		inline bool URLToString(
			const String& url,
			basic_string<_Elem, _Traits, _Alloc>& str,
			const String& header = TEXT(""),
			const DWORD flagsOpenURL = 0,
			const InternetStatus* internetStatusOpenURL = nullptr,
			const DWORD flagsReadFileEx = 0,
			const DWORD_PTR contextReadFileEx = 0
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

			auto buffer = new _Elem[mBufferSize];

			INTERNET_BUFFERS bufferInternet{};
			bufferInternet.dwStructSize = sizeof(INTERNET_BUFFERS);
			bufferInternet.lpvBuffer = buffer;

			BOOL result;
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

				str.append(buffer, bufferInternet.dwBufferLength);
			} while (true);

			delete[] buffer;
			if (internetStatusOpenURL)
			{
				internetStatusOpenURL->ResetInstance(hInternetOpenUrl);
			}
			InternetCloseHandle(hInternetOpenUrl);

			return result;
		}

		template <class _Elem = char, class _Traits = char_traits<_Elem>, class _Alloc = allocator<_Elem>>
		inline future<bool> URLToStringAsync(
			const String& url,
			basic_string<_Elem, _Traits, _Alloc>& str,
			const String& header = TEXT(""),
			const DWORD flagsOpenURL = 0,
			const InternetStatus* internetStatusOpenURL = nullptr,
			const DWORD flagsReadFileEx = 0,
			const DWORD_PTR contextReadFileEx = 0
		) {
			return async(launch::async, [=, &str]() {
				return URLToString(
					url,
					str,
					header,
					flagsOpenURL,
					internetStatusOpenURL,
					flagsReadFileEx,
					contextReadFileEx
				);
				});
		}

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
