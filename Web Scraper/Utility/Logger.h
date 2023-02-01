#pragma once

#include "Shared/String.h"

using namespace Shared;

#define TRACE_LOCATION_PROCESS_THREAD_ID String(TEXT("PID: ") + ToString(::GetCurrentProcessId()) + TEXT("\tTID: ") + ToString(::GetCurrentThreadId()))
#define TRACE_LOCATION_FILE_LINE String(String(TEXT(__FILE__)) + TEXT(":") + ToString(__LINE__))
#define TRACE_LOCATION String(TRACE_LOCATION_PROCESS_THREAD_ID + TEXT("\t") + TRACE_LOCATION_FILE_LINE)
#define TRACE(strn) Utility::Print<const ::TCHAR*>((TRACE_LOCATION + TEXT("\t\t") + (StringStream() << strn).str()).c_str())

namespace Utility {
    template<typename Object, typename Iterable>
    void Print(
        const Iterable& iterable,
        const String& separatorDimensions = TEXT("\n"),
        const function<void(const Object&)>& funcPrintElem = [] (const auto& obj) {
            if constexpr (is_arithmetic_v<decay_t<Object>>) {
                ::OutputDebugString(ToString(obj).c_str());
            } else if constexpr (is_same_v<remove_const_t<remove_reference_t<decay_t<Object>>>, String>) {
                ::OutputDebugString(obj.c_str());
            } else if constexpr (is_same_v<remove_const_t<remove_pointer_t<decay_t<Object>>>, ::TCHAR>) {
                ::OutputDebugString(obj);
            } else {
                static_assert(false, R"(The object from the innermost range is not a built-in/(c)string type, please provide a valid print element function.)");
            }

            ::OutputDebugString(TEXT(" "));
        }
    ) {
        if constexpr (ranges::range<Iterable>) {
            ::OutputDebugString(separatorDimensions.c_str());
            ranges::for_each(iterable, [&] (const auto& it) { Print(it, separatorDimensions, funcPrintElem); });
        } else {
            funcPrintElem(iterable);
            ::OutputDebugString(separatorDimensions.c_str());
        }
    }
}