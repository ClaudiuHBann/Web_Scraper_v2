#include "pch.h"
#include "GUID.h"

namespace Utility {
    GUID::GUID()
        : mStatus(::UuidCreate(&mUUID))
        , mIsGlobal(mStatus == RPC_S_UUID_LOCAL_ONLY) {}

    GUID::~GUID() {
        if (mUUIDString && IsGood()) {
            ::RpcStringFree(&mUUIDString);
        }
    }

    bool GUID::IsGlobal() const {
        return mIsGlobal;
    }

    String GUID::GetString() {
        return ToString((const ::TCHAR*)GetStr());
    }

    const GUID::RPC_STR& GUID::GetStr() {
        if (!mUUIDString && IsGood()) {
            mStatus = ::UuidToString(&mUUID, &mUUIDString);
            mUUIDStringTryCreate = true;
        }

        return IsGood() ? mUUIDString : mUUIDStringDefault;
    }

    const ::UUID& GUID::Get() const {
        return IsGood() ? mUUID : mUUIDDefault;
    }

    bool GUID::IsGood() const {
        if (mUUIDStringTryCreate) {
            return mStatus != RPC_S_OUT_OF_MEMORY;
        } else {
            if (mIsGlobal) {
                return true;
            } else {
                return mStatus != RPC_S_UUID_NO_ADDRESS;
            }
        }
    }
}
