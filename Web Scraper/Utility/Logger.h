#pragma once

#include "String.h"

#define TRACE_LOCATION_PROCESS_THREAD_ID String(TEXT("PID: ") + ToString(GetCurrentProcessId()) + TEXT(" TID: ") + ToString(GetCurrentThreadId()))
#define TRACE_LOCATION_FILE_LINE String(String(TEXT(__FILE__)) + TEXT(":") + ToString(__LINE__))
#define TRACE_LOCATION String(TRACE_LOCATION_PROCESS_THREAD_ID + TEXT(" ") + TRACE_LOCATION_FILE_LINE)
#define TRACE(str) Print<const TCHAR*>((TRACE_LOCATION + TEXT(" ") + ToString(str)).c_str())

template<typename Object, typename Iterable>
void Print(
    const Iterable& iterable,
    const String& separatorDimensions = TEXT("\n"),
    const function<void(const Object&)>& funcPrintElem = [] (const auto& obj) {
        static_assert(
            is_arithmetic_v<Object> || is_same_v<remove_const_t<remove_pointer_t<Object>>, TCHAR>,
            R"(The object from the innermost range is not a built-in/c-string type, please provide a valid print element function.)"
            );

        OutputDebugString(ToString(obj).c_str());
        OutputDebugString(TEXT(" "));
    }
) {
    if constexpr (ranges::range<Iterable>) {
        OutputDebugString(separatorDimensions.c_str());
        ranges::for_each(iterable, [&] (const auto& it) { Print(it, separatorDimensions, funcPrintElem); });
    } else {
        funcPrintElem(iterable);
        OutputDebugString(separatorDimensions.c_str());
    }
}