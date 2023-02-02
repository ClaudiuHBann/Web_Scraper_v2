#include "pch.h"
#include "File.h"

namespace Shared {
    /* static */ bool File::Exists(const String& file) {
        return exists(file);
    }

    /* static */ String File::ReadAllText(const String& file) {
        if (!Exists(file)) {
            return {};
        }

        IfStream stream(file);
        return String((istreambuf_iterator<::TCHAR>(stream)), istreambuf_iterator<::TCHAR>());
    }
}
