#pragma once

namespace Scraper {
    class DownloadProgress : public IBindStatusCallback {
    public:
        void SetCallback(function<HRESULT(IBinding*)> callbackOnStartBinding);
        void SetCallback(function<HRESULT(LONG*)> callbackGetPriority);
        void SetCallback(function<HRESULT(ULONG, ULONG, ULONG, LPCWSTR)> callbackOnProgress);
        void SetCallback(function<HRESULT(HRESULT, LPCWSTR)> callbackOnStopBinding);
        void SetCallback(function<HRESULT(DWORD*, BINDINFO*)> callbackGetBindInfo);
        void SetCallback(function<HRESULT(DWORD, DWORD, FORMATETC*, STGMEDIUM*)> callbackOnDataAvailable);
        void SetCallback(function<HRESULT(REFIID, IUnknown*)> callbackOnObjectAvailable);

    private:
        function<HRESULT(IBinding*)> mCallbackOnStartBinding = [] (auto*) { return S_OK; };
        function<HRESULT(LONG*)> mCallbackGetPriority = [] (auto*) { return S_OK; };
        function<HRESULT(ULONG, ULONG, ULONG, LPCWSTR)> mCallbackOnProgress = [] (auto, auto, auto, const auto*) { return S_OK; };
        function<HRESULT(HRESULT, LPCWSTR)> mCallbackOnStopBinding = [] (auto, const auto*) { return S_OK; };
        function<HRESULT(DWORD*, BINDINFO*)> mCallbackGetBindInfo = [] (auto*, auto*) { return S_OK; };
        function<HRESULT(DWORD, DWORD, FORMATETC*, STGMEDIUM*)> mCallbackOnDataAvailable = [] (auto, auto, auto*, auto*) { return S_OK; };
        function<HRESULT(REFIID, IUnknown*)> mCallbackOnObjectAvailable = [] (const auto&, auto*) { return S_OK; };

        HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, __RPC__in_opt IBinding* pib) override;
        HRESULT STDMETHODCALLTYPE GetPriority(__RPC__out LONG* pnPriority) override;
        HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved) override;
        HRESULT STDMETHODCALLTYPE OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, __RPC__in_opt LPCWSTR szStatusText) override;
        HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, __RPC__in_opt LPCWSTR szError) override;
        HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD* grfBINDF, BINDINFO* pbindinfo) override;
        HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC* pformatetc, STGMEDIUM* pstgmed) override;
        HRESULT STDMETHODCALLTYPE OnObjectAvailable(__RPC__in REFIID riid, __RPC__in_opt IUnknown* punk) override;
    };
}