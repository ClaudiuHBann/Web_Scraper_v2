#pragma once

#include "Shared/String.h"

using namespace Shared;

namespace Parser {
    String FindElementSpecial(const String& data, const String& elementName, const pair<String, String>& elementAttributeValuePairs = {}, const size_t offset = 0);
    String FindElement(const String& data, const String& elementName, const pair<String, String>& elementAttributeValuePairs = {}, const size_t offset = 0);

    String FindAttributeValue(const String& element, const String& attribute, const size_t offset = 0);
}