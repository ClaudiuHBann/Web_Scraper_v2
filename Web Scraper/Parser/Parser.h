#pragma once

#include "Shared/String.h"

using namespace Shared;

namespace Parser {
    vector<size_t> FindAllOccurrences(const String& data, const String& query);

    String GetAttributeValue(const String& element, const String& attribute, const size_t offset = 0);

    vector<String> Split(const String& data, const String& delimiter);
}