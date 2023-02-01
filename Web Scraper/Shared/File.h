#pragma once

#include "String.h"

namespace Shared {
    class File {
    public:
        static inline bool Exists(const String& file) {
            return exists(file);
        }

        static inline String ReadAllText(const String& file) {
            if (!Exists(file)) {
                return String();
            }

            IfStream stream(file);
            return String((istreambuf_iterator<::TCHAR>(stream)), istreambuf_iterator<::TCHAR>());
        }
    };
}
