#pragma once

#include "Utility/Logger.h"

namespace Scraper {
	namespace Progress {
		class BindStatus : public IBindStatusCallback {
		public:
			void SetCallback(function<HRESULT(IBinding*)> callbackOnStartBinding);
			void SetCallback(function<HRESULT(LONG*)> callbackGetPriority);
			void SetCallback(function<HRESULT(ULONG, ULONG, ULONG, LPCWSTR)> callbackOnProgress);
			void SetCallback(function<HRESULT(HRESULT, LPCWSTR)> callbackOnStopBinding);
			void SetCallback(function<HRESULT(DWORD*, BINDINFO*)> callbackGetBindInfo);
			void SetCallback(function<HRESULT(DWORD, DWORD, FORMATETC*, STGMEDIUM*)> callbackOnDataAvailable);
			void SetCallback(function<HRESULT(REFIID, IUnknown*)> callbackOnObjectAvailable);

		private:
			function<HRESULT(ULONG, ULONG, ULONG, LPCWSTR)> mCallbackOnProgress = [] (ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR wszStatusText) {
				TRACE(TEXT("OnProgess: Downloaded ") << ulProgress << TEXT(" of ") << ulProgressMax << TEXT(" byte(s). Status: ") << (wszStatusText ? wszStatusText : TEXT("-")) << TEXT(" (") << ulStatusCode << TEXT(")"));
				return S_OK;
			};

			function<HRESULT(HRESULT, LPCWSTR)> mCallbackOnStopBinding = [] (HRESULT hresult, LPCWSTR szError) {
				TRACE(TEXT("OnStopBinding: Status: ") << (szError ? szError : TEXT("-")) << TEXT(" (") << hresult << TEXT(")"));
				return S_OK;
			};

			function<HRESULT(IBinding*)> mCallbackOnStartBinding = [] (auto*) { return S_OK; };
			function<HRESULT(LONG*)> mCallbackGetPriority = [] (auto*) { return S_OK; };
			function<HRESULT(DWORD*, BINDINFO*)> mCallbackGetBindInfo = [] (auto*, auto*) { return S_OK; };
			function<HRESULT(DWORD, DWORD, FORMATETC*, STGMEDIUM*)> mCallbackOnDataAvailable = [] (auto, auto, auto*, auto*) { return S_OK; };
			function<HRESULT(REFIID, IUnknown*)> mCallbackOnObjectAvailable = [] (const auto&, auto*) { return S_OK; };

			HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, __RPC__in_opt IBinding* pib) override;
			HRESULT STDMETHODCALLTYPE GetPriority(__RPC__out LONG* pnPriority) override;
			HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved) override;
			HRESULT STDMETHODCALLTYPE OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, __RPC__in_opt LPCWSTR wszStatusText) override;
			HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, __RPC__in_opt LPCWSTR szError) override;
			HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD* grfBINDF, BINDINFO* pbindinfo) override;
			HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC* pformatetc, STGMEDIUM* pstgmed) override;
			HRESULT STDMETHODCALLTYPE OnObjectAvailable(__RPC__in REFIID riid, __RPC__in_opt IUnknown* punk) override;

			HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
			ULONG STDMETHODCALLTYPE AddRef(void) override;
			ULONG STDMETHODCALLTYPE Release(void) override;
		};
	}
}
