#pragma once

#include "String.h"

namespace Shared {
    class File {
    public:
        static bool Exists(const String& file);

        static String ReadAllText(const String& file);
    };
}
