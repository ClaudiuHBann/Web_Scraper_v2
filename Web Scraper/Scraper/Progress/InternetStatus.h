#pragma once

namespace Scraper {
	namespace Progress {
		class InternetStatus {
		public:
			bool SetInstance(HINTERNET hInternet) const;
			void ResetInstance(HINTERNET hInternet) const;

			function<void(HINTERNET, DWORD_PTR, DWORD, LPVOID, DWORD)> GetCallback() const;
			void SetCallback(function<void(HINTERNET, DWORD_PTR, DWORD, LPVOID, DWORD)> callback);

			DWORD_PTR GetContext() const;
			void SetContext(const DWORD_PTR context);

		private:
			function<void(HINTERNET, DWORD_PTR, DWORD, LPVOID, DWORD)> mCallback{};
			DWORD_PTR mContext{};
		};
	}
}
