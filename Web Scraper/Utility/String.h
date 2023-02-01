#pragma once

namespace Utility {
#if defined(_UNICODE) || defined(UNICODE)
    using String = wstring;
#else
    using String = string;
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
}
