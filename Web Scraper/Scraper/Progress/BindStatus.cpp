#include "pch.h"
#include "BindStatus.h"
#include "Utility/Logger.h"

namespace Scraper {
	namespace Progress {
		void BindStatus::SetCallback(function<HRESULT(IBinding*)> callbackOnStartBinding) {
			mCallbackOnStartBinding = callbackOnStartBinding;
		}

		void BindStatus::SetCallback(function<HRESULT(LONG*)> callbackGetPriority) {
			mCallbackGetPriority = callbackGetPriority;
		}

		void BindStatus::SetCallback(function<HRESULT(ULONG, ULONG, ULONG, LPCWSTR)> callbackOnProgress) {
			mCallbackOnProgress = callbackOnProgress;
		}

		void BindStatus::SetCallback(function<HRESULT(HRESULT, LPCWSTR)> callbackOnStopBinding) {
			mCallbackOnStopBinding = callbackOnStopBinding;
		}

		void BindStatus::SetCallback(function<HRESULT(DWORD*, BINDINFO*)> callbackGetBindInfo) {
			mCallbackGetBindInfo = callbackGetBindInfo;
		}

		void BindStatus::SetCallback(function<HRESULT(DWORD, DWORD, FORMATETC*, STGMEDIUM*)> callbackOnDataAvailable) {
			mCallbackOnDataAvailable = callbackOnDataAvailable;
		}

		void BindStatus::SetCallback(function<HRESULT(REFIID, IUnknown*)> callbackOnObjectAvailable) {
			mCallbackOnObjectAvailable = callbackOnObjectAvailable;
		}

		HRESULT __stdcall BindStatus::OnStartBinding(DWORD /*dwReserved*/, IBinding* pib) {
			return mCallbackOnStartBinding(pib);
		}

		HRESULT __stdcall BindStatus::GetPriority(LONG* pnPriority) {
			return mCallbackGetPriority(pnPriority);
		}

		HRESULT __stdcall BindStatus::OnLowResource(DWORD reserved) {
			TRACE("BindStatus::OnLowResource(" << reserved << ")");
			return E_NOTIMPL;
		}

		HRESULT __stdcall BindStatus::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText) {
			return mCallbackOnProgress(ulProgress, ulProgressMax, ulStatusCode, szStatusText);
		}

		HRESULT __stdcall BindStatus::OnStopBinding(HRESULT hresult, LPCWSTR szError) {
			return mCallbackOnStopBinding(hresult, szError);
		}

		HRESULT __stdcall BindStatus::GetBindInfo(DWORD* grfBINDF, BINDINFO* pbindinfo) {
			return mCallbackGetBindInfo(grfBINDF, pbindinfo);
		}

		HRESULT __stdcall BindStatus::OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC* pformatetc, STGMEDIUM* pstgmed) {
			return mCallbackOnDataAvailable(grfBSCF, dwSize, pformatetc, pstgmed);
		}

		HRESULT __stdcall BindStatus::OnObjectAvailable(REFIID riid, IUnknown* punk) {
			return mCallbackOnObjectAvailable(riid, punk);
		}

		HRESULT __stdcall BindStatus::QueryInterface(REFIID /*riid*/, void** /*ppvObject*/) {
			return S_OK;
		}

		ULONG __stdcall BindStatus::AddRef(void) {
			return 0;
		}

		ULONG __stdcall BindStatus::Release(void) {
			return 0;
		}
	}
}
