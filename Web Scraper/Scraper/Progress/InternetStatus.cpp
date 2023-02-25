#include "pch.h"
#include "InternetStatus.h"
#include "Utility/Logger.h"

namespace Scraper {
	namespace Progress {
		bool InternetStatus::SetInstance(HINTERNET hInternet) const {
			if (!hInternet || !mCallback)
			{
				return false;
			}

			auto target = mCallback.target<void(*)(HINTERNET, DWORD_PTR, DWORD, LPVOID, DWORD)>();
			assert(target && "Decay your lambda like +[](){} !");

			return InternetSetStatusCallback(hInternet, *target) != INTERNET_INVALID_STATUS_CALLBACK;
		}

		void InternetStatus::ResetInstance(HINTERNET hInternet) const {
			if (!hInternet)
			{
				return;
			}

			InternetSetStatusCallback(hInternet, nullptr);
		}

		function<void(HINTERNET, DWORD_PTR, DWORD, LPVOID, DWORD)> InternetStatus::GetCallback() const {
			return mCallback;
		}

		void InternetStatus::SetCallback(function<void(HINTERNET, DWORD_PTR, DWORD, LPVOID, DWORD)> callback) {
			mCallback = callback;
		}

		DWORD_PTR InternetStatus::GetContext() const {
			return mContext;
		}

		void InternetStatus::SetContext(const DWORD_PTR context) {
			mContext = context;
		}
	}
}
