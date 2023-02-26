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
}
