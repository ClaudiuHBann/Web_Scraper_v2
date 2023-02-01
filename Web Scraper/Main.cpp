#include "pch.h"
#include "Utility/Utility.h"

int main() {
    Stopwatch stopwatch(TEXT("test"), true);

    Utility::GUID guid;
    TRACE_EX(guid.GetString() << " test" << " more");

    TRACE(stopwatch.GetTimeElapsed(TEXT("test")).count());

    return 0;
}