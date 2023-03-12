#pragma once

#include "Shared/String.h"

namespace Parser {
	using namespace Shared;

	class HTMLParser {
	public:
		HTMLParser();
		~HTMLParser();

		bool Parse(const String& html);

		IHTMLElement* GetElementById(const String& id);
		vector<IHTMLElement*> GetElementsByAttributes(const vector<pair<String, String>>& attributes);
		IHTMLElementCollection* GetElementsByTagNameFromCollection(const String& name);

		static String GetElementInnerHTML(IHTMLElement*& element);
		static IHTMLElement* GetElementFromCollectionByIndex(IHTMLElementCollection*& collection, const long index);
		static IHTMLElementCollection* GetElementAllAsCollection(IHTMLElement*& element);
		static vector<IHTMLElement*> GetCollectionElementsByAttributes(IHTMLElementCollection*& collection, const vector<pair<String, String>>& attributes);
		static String GetElementAttributeValueByName(IHTMLElement*& element, const String& name);

		static bool ElementHasAttribute(IHTMLElement*& element, const pair<String, String>& attribute);
		static bool ElementHasAttributes(IHTMLElement*& element, const vector<pair<String, String>>& attributes);

		String GetScriptAsString(const long index);

		IHTMLDocument2* GetIHTMLDocument2();
		IHTMLDocument3* GetIHTMLDocument3();

	private:
		IHTMLDocument2* mIHTMLDocument2 {};
		IHTMLDocument3* mIHTMLDocument3 {};

		bool Initialize2();
		bool Uninitialize2();

		bool Initialize3();
		bool Uninitialize3();
	};
}