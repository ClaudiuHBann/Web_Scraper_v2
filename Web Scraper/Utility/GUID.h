#pragma once

#include "String.h"

namespace Utility {
    class GUID {
#if defined(_UNICODE) || defined(UNICODE)
        using RPC_STR = ::RPC_WSTR;
#else
        using RPC_STR = ::RPC_STR;
#endif // UNICODE

    public:
        GUID();
        ~GUID();

        bool IsGlobal() const;

        String GetString();
        const RPC_STR& GetStr();
        const ::UUID& Get() const;

    private:
        bool IsGood() const;

        ::UUID mUUID {};
        static inline ::UUID mUUIDDefault {};

        RPC_STR mUUIDString = nullptr;
        static inline RPC_STR mUUIDStringDefault = (RPC_STR)TEXT("");

        ::RPC_STATUS mStatus = 0;
        bool mIsGlobal = true;
        bool mUUIDStringTryCreate = false;
    };
}
