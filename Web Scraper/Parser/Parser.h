#pragma once

#include "Shared/String.h"

namespace Parser {
	using namespace Shared;

	class HTMLParser
	{
	public:
		HTMLParser(const String& html = {});
		~HTMLParser();

		IHTMLDocument2* GetIHTMLDocument2();

		bool SetHTML(const String& html);

	private:
		bool mInitialized{};
		bool mReset{};
		SAFEARRAY* mSafeArray{};
		VARIANT* mVariant{};
		IHTMLDocument2* mIHTMLDocument2{};

		void Reset();
	};
}