#include "pch.h"
#include "Stopwatch.h"
#include "String.h"

Stopwatch::Stopwatch(const String& name, const bool start /* = false */) {
    Create(name, start);
}

size_t Stopwatch::Size() {
    scoped_lock guard(mMutex);
    return mStopwatches.size();
}

bool Stopwatch::Create(const String& name, const bool start /* = false */, const bool overwrite /* = false */) {
    if (name.empty()) {
        return false;
    }

    scoped_lock guard(mMutex);
    if (mStopwatches.find(name) != mStopwatches.end() && !overwrite) {
        return false;
    }

    mStopwatches[name] = { start ? steady_clock::now() : steady_clock::time_point(), start };
    return true;
}

bool Stopwatch::Pause(const String& name) {
    scoped_lock guard(mMutex);
    if (mStopwatches.find(name) != mStopwatches.end()) {
        // a paused stopwatch will contain the elapsed time
        mStopwatches[name] = { steady_clock::time_point(GetTimeElapsed(name)), false };
        return true;
    }

    return false;
}

bool Stopwatch::Resume(const String& name) {
    scoped_lock guard(mMutex);
    if (mStopwatches.find(name) != mStopwatches.end() && !mStopwatches[name].second) {
        // a paused stopwatch contains the elapsed time
        // so we need to take the current time and subtract from current moment
        steady_clock::time_point nowBackwards(steady_clock::now() - mStopwatches[name].first);
        mStopwatches[name] = { nowBackwards, true };
        return true;
    }

    return false;
}

bool Stopwatch::Remove(const String& name) {
    scoped_lock guard(mMutex);
    return mStopwatches.erase(name);
}

bool Stopwatch::Reset(const String& name, const bool start /* = true */) {
    return Create(name, start, true);
}

nanoseconds Stopwatch::GetTimeElapsed(const String& name) {
    scoped_lock guard(mMutex);
    if (mStopwatches.find(name) != mStopwatches.end()) {
        if (mStopwatches[name].second) {
            return steady_clock::now() - mStopwatches[name].first;
        } else {
            // a paused stopwatch already contains the elapsed time
            return mStopwatches[name].first.time_since_epoch();
        }
    }

    return {};
}
