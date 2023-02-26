#pragma once

#include "Shared/String.h"

using namespace Shared;

namespace Parser {
	class HTMLParser
	{
	public:
		HTMLParser();
		~HTMLParser();

		IHTMLDocument2* GetIHTMLDocument2();

		bool SetHTML(const String& html);

	private:
		bool mInitialized{};
		IHTMLDocument2* mIHTMLDocument2{};
	};
}