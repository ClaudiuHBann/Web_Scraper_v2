#pragma once

#include "String.h"

namespace Utility {
    class Stopwatch {
    public:
        inline Stopwatch() = default;
        Stopwatch(const String& name, const bool start = false);

        size_t Size();

        bool Create(const String& name, const bool start = false, const bool overwrite = false);
        bool Pause(const String& name);
        bool Resume(const String& name);
        bool Reset(const String& name, const bool start = true);
        bool Remove(const String& name);

        nanoseconds GetTimeElapsed(const String& name);

    protected:
        map<String, pair<steady_clock::time_point, bool>> mStopwatches;
        //  name         time now/elapsed          running

    private:
        mutex mMutex;
    };
}
