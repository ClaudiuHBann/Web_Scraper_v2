#include "pch.h"
#include "Parser.h"

namespace Parser {
	HTMLParser::HTMLParser(const String& html /* = {} */) {
		auto result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		mInitialized = result == S_OK || result == S_FALSE || result == RPC_E_CHANGED_MODE;

		if (mInitialized)
		{
			result = CoCreateInstance(CLSID_HTMLDocument, nullptr, CLSCTX_INPROC_SERVER, IID_IHTMLDocument2, (void**)&mIHTMLDocument2);
			if (!html.empty() && SUCCEEDED(result))
			{
				SetHTML(html);
			}
		}
	}

	HTMLParser::~HTMLParser() {
		Reset();

		if (mIHTMLDocument2)
		{
			mIHTMLDocument2->Release();
		}

		if (mInitialized)
		{
			CoUninitialize();
		}
	}

	IHTMLDocument2* HTMLParser::GetIHTMLDocument2() {
		return mIHTMLDocument2;
	}

	bool HTMLParser::SetHTML(const String& html) {
		if (!mIHTMLDocument2)
		{
			return false;
		}

		Reset();

		CString htmlCString(html.c_str());

		mSafeArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);
		if (!mSafeArray)
		{
			return false;
		}

		auto result = SafeArrayAccessData(mSafeArray, (LPVOID*)&mVariant);
		if (!mVariant)
		{
			SafeArrayDestroy(mSafeArray);
			return false;
		}

		mVariant->vt = VT_BSTR;
		mVariant->bstrVal = htmlCString.AllocSysString();

		result = mIHTMLDocument2->write(mSafeArray);

		mReset = true;
		return SUCCEEDED(result);
	}

	void HTMLParser::Reset() {
		if (!mReset)
		{
			return;
		}

		mIHTMLDocument2->close();

		if (mVariant && mVariant->bstrVal)
		{
			SysFreeString(mVariant->bstrVal);
		}

		if (mSafeArray)
		{
			//SafeArrayUnaccessData(mSafeArray);
			SafeArrayDestroy(mSafeArray);
		}

		mReset = false;
	}

	/* static */ IHTMLElementCollection* HTMLParser::GetElementAllAsCollection(IHTMLElement*& element) {
		IDispatch* childrenRaw{};
		auto result = element->get_all(&childrenRaw);
		if (FAILED(result))
		{
			return nullptr;
		}

		IHTMLElementCollection* children{};
		result = childrenRaw->QueryInterface(IID_IHTMLElementCollection, reinterpret_cast<void**>(&children));
		return children;
	}

	/* static */ vector<IHTMLElement*> HTMLParser::GetElementChildren(IHTMLElement*& element, const vector<pair<String, String>>& attributes /* = {} */) {
		IDispatch* childrenRaw{};
		auto result = element->get_children(&childrenRaw);
		if (FAILED(result))
		{
			return {};
		}

		IHTMLElementCollection* children{};
		result = childrenRaw->QueryInterface(IID_IHTMLElementCollection, reinterpret_cast<void**>(&children));
		if (FAILED(result))
		{
			return {};
		}

		return GetCollectionElementsByAttributes(children, attributes);
	}

	/* static */ vector<IHTMLElement*> HTMLParser::GetCollectionElementsByAttributes(IHTMLElementCollection*& collection, const vector<pair<String, String>>& attributes) {
		vector<IHTMLElement*> htmlElements{};

		long collectionLength;
		collection->get_length(&collectionLength);
		for (long i = 0; i < collectionLength; i++)
		{
			auto element = GetElementFromCollectionByIndex(collection, i);
			if (element && ElementHasAttributes(element, attributes))
			{
				htmlElements.push_back(element);
			}
		}

		return htmlElements;
	}

	/* static */ IHTMLElement* HTMLParser::GetElementFromCollectionByIndex(IHTMLElementCollection*& collection, const long index) {
		VARIANT variantName{};
		variantName.vt = VT_UINT;
		variantName.lVal = index;

		VARIANT variantIndex{};
		variantIndex.vt = VT_I4;
		variantIndex.intVal = 0;

		IDispatch* iDispatch{};
		auto result = collection->item(variantName, variantIndex, &iDispatch);
		if (FAILED(result))
		{
			return nullptr;
		}

		IHTMLElement* element{};
		result = iDispatch->QueryInterface(IID_IHTMLElement, (void**)&element);
		return element;
	}

	/* static */ bool HTMLParser::ElementHasAttribute(IHTMLElement*& element, const pair<String, String>& attribute) {
		return GetElementAttributeValueByName(element, attribute.first) == attribute.second;
	}

	/* static */ bool HTMLParser::ElementHasAttributes(IHTMLElement*& element, const vector<pair<String, String>>& attributes) {
		for (const auto& attribute : attributes)
		{
			if (!ElementHasAttribute(element, attribute))
			{
				return false;
			}
		}

		return true;
	}

	String HTMLParser::GetScriptAsString(const long index) {
		IHTMLElementCollection* scripts{};
		mIHTMLDocument2->get_scripts(&scripts);
		if (!scripts)
		{
			return {};
		}

		auto script = GetElementFromCollectionByIndex(scripts, index);
		if (!script)
		{
			return {};
		}

		BSTR bstr;
		script->get_innerHTML(&bstr);
		return bstr;
	}

	vector<IHTMLElement*> HTMLParser::GetElementsByAttributes(const vector<pair<String, String>>& attributes) {
		IHTMLElementCollection* collection{};
		auto result = mIHTMLDocument2->get_all(&collection);
		if (FAILED(result))
		{
			return {};
		}

		return GetCollectionElementsByAttributes(collection, attributes);
	}

	/* static */ vector<IHTMLElement*> HTMLParser::GetElementNthChildrenGeneration(IHTMLElement*& element, long generation) {
		auto children = GetElementChildren(element);

		while (generation--)
		{
			children = GetElementChildren(children.front());
		}

		return children;
	}

	/* static */ String HTMLParser::GetElementAttributeValueByName(IHTMLElement*& element, const String& name) {
		VARIANT variantAttribute{};
		variantAttribute.vt = VT_BSTR;

		CString attributeNameCString(name.c_str());
		auto attributeNameBStr = attributeNameCString.AllocSysString();

		auto result = element->getAttribute(attributeNameBStr, 0, &variantAttribute);
		SysFreeString(attributeNameBStr);

		if (SUCCEEDED(result) && variantAttribute.vt == VT_BSTR && variantAttribute.bstrVal) {
			return variantAttribute.bstrVal;
		}
		else {
			return {};
		}
	}
}
