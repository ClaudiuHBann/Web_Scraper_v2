#include "pch.h"
#include "Parser.h"

namespace Parser {
	thread_local size_t HTMLParserInstanceCount = 0;

	HTMLParser::HTMLParser() {
		if (!HTMLParserInstanceCount++) {
			auto result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
			if (result != S_OK && result != S_FALSE && result != RPC_E_CHANGED_MODE) {
				HTMLParserInstanceCount--;
			}
		}
	}

	HTMLParser::~HTMLParser() {
		Uninitialize3();
		Uninitialize2();

		if (!--HTMLParserInstanceCount) {
			CoUninitialize();
		}
	}

	IHTMLDocument2* HTMLParser::GetIHTMLDocument2() {
		Initialize2();

		return mIHTMLDocument2;
	}

	IHTMLDocument3* HTMLParser::GetIHTMLDocument3() {
		Initialize3();

		return mIHTMLDocument3;
	}

	IHTMLDocument7* HTMLParser::GetIHTMLDocument7() {
		Initialize7();

		return mIHTMLDocument7;
	}

	bool HTMLParser::Parse(const String& html) {
		if (!Initialize2()) {
			return false;
		}

		CString htmlCString(html.c_str());

		auto safeArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);
		if (!safeArray) {
			return false;
		}

		VARIANT* variant {};
		auto result = SafeArrayAccessData(safeArray, (void**)&variant);
		if (!variant) {
			SafeArrayDestroy(safeArray);
			return false;
		}

		variant->vt = VT_BSTR;
		variant->bstrVal = htmlCString.AllocSysString();

		result = mIHTMLDocument2->write(safeArray);
		mIHTMLDocument2->close();

		SysFreeString(variant->bstrVal);
		SafeArrayDestroy(safeArray);

		return SUCCEEDED(result);
	}

	/* static */ IHTMLElementCollection* HTMLParser::GetElementAllAsCollection(IHTMLElement* element) {
		if (!element) {
			return nullptr;
		}

		IDispatch* childrenRaw {};
		auto result = element->get_all(&childrenRaw);
		if (FAILED(result)) {
			return nullptr;
		}

		IHTMLElementCollection* children {};
		result = childrenRaw->QueryInterface(IID_IHTMLElementCollection, reinterpret_cast<void**>(&children));
		return children;
	}

	/* static */ IHTMLElement* HTMLParser::GetElementFromCollectionByIndex(IHTMLElementCollection* collection, const long index) {
		if (!collection) {
			return nullptr;
		}

		VARIANT variantName {};
		variantName.vt = VT_UINT;
		variantName.lVal = index;

		VARIANT variantIndex {};
		variantIndex.vt = VT_I4;
		variantIndex.intVal = 0;

		IDispatch* iDispatch {};
		auto result = collection->item(variantName, variantIndex, &iDispatch);
		if (!iDispatch) {
			return nullptr;
		}

		IHTMLElement* element {};
		result = iDispatch->QueryInterface(IID_IHTMLElement, (void**)&element);
		return element;
	}

	/* static */ bool HTMLParser::ElementHasAttribute(IHTMLElement* element, const pair<String, String>& attribute) {
		if (!element) {
			return false;
		}

		return GetElementAttributeValueByName(element, attribute.first) == attribute.second;
	}

	/* static */ bool HTMLParser::ElementHasAttributes(IHTMLElement* element, const vector<pair<String, String>>& attributes) {
		if (!element) {
			return false;
		}

		for (const auto& attribute : attributes) {
			if (!ElementHasAttribute(element, attribute)) {
				return false;
			}
		}

		return true;
	}

	String HTMLParser::GetScriptAsString(const long index) {
		IHTMLElementCollection* scripts {};
		mIHTMLDocument2->get_scripts(&scripts);
		if (!scripts) {
			return {};
		}

		auto script = GetElementFromCollectionByIndex(scripts, index);
		if (!script) {
			return {};
		}

		return GetElementInnerHTML(script);
	}

	/* static */ String HTMLParser::GetElementInnerHTML(IHTMLElement* element) {
		if (!element) {
			return String();
		}

		BSTR bstr;
		element->get_innerHTML(&bstr);
		return bstr;
	}

	IHTMLElement* HTMLParser::GetElementById(const String& id) {
		BSTR bStr = SysAllocString(id.c_str());
		IHTMLElement* element {};
		GetIHTMLDocument3()->getElementById(bStr, &element);
		SysFreeString(bStr);
		return element;
	}

	vector<IHTMLElement*> HTMLParser::GetElementsByAttributes(const vector<pair<String, String>>& attributes) {
		IHTMLElementCollection* collection {};
		auto result = mIHTMLDocument2->get_all(&collection);
		if (FAILED(result)) {
			return {};
		}

		return GetCollectionElementsByAttributes(collection, attributes);
	}

	/* static */ vector<IHTMLElement*> HTMLParser::GetCollectionElementsByAttributes(IHTMLElementCollection* collection, const vector<pair<String, String>>& attributes) {
		if (!collection) {
			return {};
		}

		vector<IHTMLElement*> htmlElements {};

		long collectionLength;
		collection->get_length(&collectionLength);
		for (long i = 0; i < collectionLength; i++) {
			auto element = GetElementFromCollectionByIndex(collection, i);
			if (element && ElementHasAttributes(element, attributes)) {
				htmlElements.push_back(element);
			}
		}

		return htmlElements;
	}

	IHTMLElementCollection* HTMLParser::GetElementsByClassName(const String& name) {
		BSTR bStr = SysAllocString(name.c_str());
		IHTMLElementCollection* collection {};
		GetIHTMLDocument7()->getElementsByClassName(bStr, &collection);
		SysFreeString(bStr);
		return collection;
	}

	/* static */ String HTMLParser::GetElementAttributeValueByName(IHTMLElement* element, const String& name) {
		if (!element) {
			return String();
		}

		VARIANT variantAttribute {};
		variantAttribute.vt = VT_BSTR;

		CString attributeNameCString(name.c_str());
		auto attributeNameBStr = attributeNameCString.AllocSysString();

		auto result = element->getAttribute(attributeNameBStr, 0, &variantAttribute);
		SysFreeString(attributeNameBStr);

		if (SUCCEEDED(result) && variantAttribute.vt == VT_BSTR && variantAttribute.bstrVal) {
			return variantAttribute.bstrVal;
		} else {
			return {};
		}
	}

	bool HTMLParser::Initialize2() {
		if (!HTMLParserInstanceCount) {
			return false;
		}

		if (mIHTMLDocument2) {
			return true;
		}

		return SUCCEEDED(CoCreateInstance(CLSID_HTMLDocument, nullptr, CLSCTX_INPROC_SERVER, IID_IHTMLDocument2, (void**)&mIHTMLDocument2));
	}

	bool HTMLParser::Uninitialize2() {
		if (!mIHTMLDocument2) {
			return true;
		}

		return !mIHTMLDocument2->Release();
	}

	bool HTMLParser::Initialize3() {
		if (!Initialize2()) {
			return false;
		}

		if (mIHTMLDocument3) {
			return true;
		}

		return SUCCEEDED(mIHTMLDocument2->QueryInterface(IID_IHTMLDocument3, (void**)&mIHTMLDocument3));
	}

	bool HTMLParser::Uninitialize3() {
		if (!mIHTMLDocument3) {
			return true;
		}

		return !mIHTMLDocument3->Release();
	}

	bool HTMLParser::Initialize7() {
		if (!Initialize2()) {
			return false;
		}

		if (mIHTMLDocument7) {
			return true;
		}

		return SUCCEEDED(mIHTMLDocument2->QueryInterface(IID_IHTMLDocument7, (void**)&mIHTMLDocument7));
	}

	bool HTMLParser::Uninitialize7() {
		if (!mIHTMLDocument7) {
			return true;
		}

		return !mIHTMLDocument7->Release();
	}
}
