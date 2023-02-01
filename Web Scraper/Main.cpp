#include "pch.h"
#include "Utility/Utility.h"

int main() {
    Stopwatch stopwatch(TEXT("test"), true);

    Utility::GUID guid;
    TRACE(guid.GetString());

    TRACE(stopwatch.GetTimeElapsed(TEXT("test")).count());

    return 0;
}