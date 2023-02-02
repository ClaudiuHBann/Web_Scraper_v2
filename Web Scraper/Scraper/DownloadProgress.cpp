#include "pch.h"
#include "DownloadProgress.h"
#include "Utility/Logger.h"

namespace Scraper {
    void DownloadProgress::SetCallback(function<HRESULT(IBinding*)> callbackOnStartBinding) {
        mCallbackOnStartBinding = callbackOnStartBinding;
    }

    void DownloadProgress::SetCallback(function<HRESULT(LONG*)> callbackGetPriority) {
        mCallbackGetPriority = callbackGetPriority;
    }

    void DownloadProgress::SetCallback(function<HRESULT(ULONG, ULONG, ULONG, LPCWSTR)> callbackOnProgress) {
        mCallbackOnProgress = callbackOnProgress;
    }

    void DownloadProgress::SetCallback(function<HRESULT(HRESULT, LPCWSTR)> callbackOnStopBinding) {
        mCallbackOnStopBinding = callbackOnStopBinding;
    }

    void DownloadProgress::SetCallback(function<HRESULT(DWORD*, BINDINFO*)> callbackGetBindInfo) {
        mCallbackGetBindInfo = callbackGetBindInfo;
    }

    void DownloadProgress::SetCallback(function<HRESULT(DWORD, DWORD, FORMATETC*, STGMEDIUM*)> callbackOnDataAvailable) {
        mCallbackOnDataAvailable = callbackOnDataAvailable;
    }

    void DownloadProgress::SetCallback(function<HRESULT(REFIID, IUnknown*)> callbackOnObjectAvailable) {
        mCallbackOnObjectAvailable = callbackOnObjectAvailable;
    }

    HRESULT __stdcall DownloadProgress::OnStartBinding(DWORD dwReserved, IBinding* pib) {
        return mCallbackOnStartBinding(pib);
    }

    HRESULT __stdcall DownloadProgress::GetPriority(LONG* pnPriority) {
        return mCallbackGetPriority(pnPriority);
    }

    HRESULT __stdcall DownloadProgress::OnLowResource(DWORD reserved) {
        TRACE("DownloadProgress::OnLowResource(" << reserved << ")");
        return E_NOTIMPL;
    }

    HRESULT __stdcall DownloadProgress::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText) {
        return mCallbackOnProgress(ulProgress, ulProgressMax, ulStatusCode, szStatusText);
    }

    HRESULT __stdcall DownloadProgress::OnStopBinding(HRESULT hresult, LPCWSTR szError) {
        return mCallbackOnStopBinding(hresult, szError);
    }

    HRESULT __stdcall DownloadProgress::GetBindInfo(DWORD* grfBINDF, BINDINFO* pbindinfo) {
        return mCallbackGetBindInfo(grfBINDF, pbindinfo);
    }

    HRESULT __stdcall DownloadProgress::OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC* pformatetc, STGMEDIUM* pstgmed) {
        return mCallbackOnDataAvailable(grfBSCF, dwSize, pformatetc, pstgmed);
    }

    HRESULT __stdcall DownloadProgress::OnObjectAvailable(REFIID riid, IUnknown* punk) {
        return mCallbackOnObjectAvailable(riid, punk);
    }
}
