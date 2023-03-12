#pragma once

#include "Utility/Utility.h"
#include "Utility/Logger.h"

namespace Scraper {
	namespace Progress {
		using namespace Utility;

		constexpr auto DEFAULT_CONTEXT = TEXT("HBann");

		class InternetStatus {
		public:
			InternetStatus(
				const function<void(HINTERNET, DWORD_PTR, DWORD, LPVOID, DWORD)>& callback = +[](
					HINTERNET hInternet,
					DWORD_PTR dwContext,
					DWORD dwInternetStatus,
					LPVOID lpvStatusInformation,
					DWORD dwStatusInformationLength
					) {
						TRACE("HINTERNET: " << GetPointerAsHexString(hInternet));
						TRACE("Context: " << dwContext);
						TRACE("Status code: " << dwInternetStatus);

						if (lpvStatusInformation) {
							TRACE("Status info code: " << *(DWORD*)lpvStatusInformation);
							TRACE("Status info size: " << dwStatusInformationLength);
						}
				},
				const DWORD_PTR context = *(DWORD_PTR*)DEFAULT_CONTEXT
					);

			bool SetInstance(HINTERNET hInternet) const;
			void ResetInstance(HINTERNET hInternet) const;

			function<void(HINTERNET, DWORD_PTR, DWORD, LPVOID, DWORD)> GetCallback() const;
			void SetCallback(function<void(HINTERNET, DWORD_PTR, DWORD, LPVOID, DWORD)> callback);

			DWORD_PTR GetContext() const;
			void SetContext(const DWORD_PTR context);

		private:
			function<void(HINTERNET, DWORD_PTR, DWORD, LPVOID, DWORD)> mCallback {};
			DWORD_PTR mContext {};
		};
	}
}
