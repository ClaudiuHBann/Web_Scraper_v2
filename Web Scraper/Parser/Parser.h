#pragma once

#include "Shared/String.h"

namespace Parser {
	using namespace Shared;

	class HTMLParser
	{
	public:
		HTMLParser(const String& html = {});
		~HTMLParser();

		bool SetHTML(const String& html);

		vector<IHTMLElement*> GetElementsByAttributes(const vector<pair<String, String>>& attributes);

		static String GetElementAttributeValueByName(IHTMLElement*& element, const String& name);

		IHTMLDocument2* GetIHTMLDocument2();

	private:
		bool mInitialized{};
		bool mReset{};
		SAFEARRAY* mSafeArray{};
		VARIANT* mVariant{};
		IHTMLDocument2* mIHTMLDocument2{};

		void Reset();

		static IHTMLElement* GetElementFromCollectionByIndex(IHTMLElementCollection*& collection, const long index);

		static bool ElementHasAttribute(IHTMLElement*& element, const pair<String, String>& attribute);
		static bool ElementHasAttributes(IHTMLElement*& element, const vector<pair<String, String>>& attributes);
	};
}