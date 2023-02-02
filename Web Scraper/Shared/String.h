#pragma once

namespace Shared {
#if defined(_UNICODE) || defined(UNICODE)
    using String = wstring;
    using StringStream = wstringstream;
    using IfStream = wifstream;
#else
    using String = string;
    using StringStream = stringstream;
    using IfStream = ifstream;
#endif // _UNICODE

    template <typename T>
    inline String ToString(const T& t) {
        if constexpr (is_same_v<remove_const_t<remove_pointer_t<decay_t<T>>>, ::TCHAR> ||
                      is_same_v<remove_const_t<remove_reference_t<decay_t<T>>>, String>) {
            return t;
        } else {
#if defined(_UNICODE) || defined(UNICODE)
            return to_wstring(t);
#else
            return to_string(t);
#endif // _UNICODE
        }
    }

    namespace StringUtil {
        String RemoveStringLead(const String& data, const String& lead);
        String StringLead(const String& data, const String& lead);

        String RemoveStringTrail(const String& data, const String& trail);
        String StringTrail(const String& data, const String& trail);

        bool Contains(const String& data, const String& query);
    }
}
