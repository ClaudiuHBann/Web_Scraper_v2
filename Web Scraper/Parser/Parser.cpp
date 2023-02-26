#include "pch.h"
#include "Parser.h"

namespace Parser {
	HTMLParser::HTMLParser() {
		auto result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		mInitialized = result == S_OK || result == S_FALSE || result == RPC_E_CHANGED_MODE;

		if (mInitialized)
		{
			result = CoCreateInstance(CLSID_HTMLDocument, nullptr, CLSCTX_INPROC_SERVER, IID_IHTMLDocument2, (void**)&mIHTMLDocument2);
		}
	}

	HTMLParser::~HTMLParser() {
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

		CString htmlCString(html.c_str());

		SAFEARRAY* safeArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);
		if (!safeArray)
		{
			return false;
		}

		VARIANT* variant{};
		auto result = SafeArrayAccessData(safeArray, (LPVOID*)&variant);
		if (!variant)
		{
			SafeArrayDestroy(safeArray);
			return false;
		}

		variant->vt = VT_BSTR;
		variant->bstrVal = htmlCString.AllocSysString();

		mIHTMLDocument2->close();
		result = mIHTMLDocument2->write(safeArray);

		SysFreeString(variant->bstrVal);
		SafeArrayUnaccessData(safeArray);
		SafeArrayDestroy(safeArray);

		return SUCCEEDED(result);
	}
}
