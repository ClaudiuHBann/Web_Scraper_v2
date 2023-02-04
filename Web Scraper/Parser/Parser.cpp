#include "pch.h"
#include "Parser.h"

namespace Parser {
    constexpr ::TCHAR QUOTE_CHAR = TEXT('"');
    constexpr size_t QUOTE_CHAR_SIZE = 1;

    constexpr ::TCHAR TAG_OPEN_START = TEXT('<');
    constexpr size_t TAG_OPEN_START_SIZE = 1;
    constexpr ::TCHAR TAG_CLOSE_START = TEXT('</');
    constexpr ::TCHAR TAG_OPEN_END = TEXT('>');
    constexpr ::TCHAR TAG_OPEN_END_SPECIAL[] = TEXT("/>");
    constexpr size_t TAG_OPEN_END_SPECIAL_SIZE = 2;
    constexpr ::TCHAR TAG_CLOSE_END = TEXT('>');
    const vector<String> TAGS_SELF_CLOSED {
        TEXT("area"),
        TEXT("base"),
        TEXT("br"),
        TEXT("col"),
        TEXT("embed"),
        TEXT("hr"),
        TEXT("img"),
        TEXT("input"),
        TEXT("link"),
        TEXT("meta"),
        TEXT("param"),
        TEXT("source"),
        TEXT("track"),
        TEXT("wbr")
    };

    String FindElementSpecial(const String& data, const String& elementName, const pair<String, String>& elementAttributeValuePairs /* = {} */, const size_t offset /* = 0 */) {
        if (ranges::find(TAGS_SELF_CLOSED, elementName) == TAGS_SELF_CLOSED.end()) {
            return {};
        }

        auto elementOpenStart = data.find(TAG_OPEN_START, offset);
        if (elementOpenStart == String::npos) {
            return {};
        }

        auto elementOpenEnd = data.find(TAG_OPEN_END_SPECIAL, elementOpenStart + elementName.size());
        if (elementOpenEnd == String::npos) {
            return {};
        }

        return data.substr(elementOpenStart, elementOpenEnd + TAG_OPEN_END_SPECIAL_SIZE - elementOpenStart);
    }

    String FindAttributeValue(const String& element, const String& attribute, const size_t offset /* = 0 */) {
        auto attributePos = element.find(attribute, offset);
        if (attributePos == String::npos) {
            return {};
        }

        auto posValueStart = element.find(QUOTE_CHAR, attributePos + attribute.size());
        if (posValueStart == String::npos) {
            return {};
        }

        auto posValueEnd = element.find(QUOTE_CHAR, posValueStart + QUOTE_CHAR_SIZE);
        if (posValueEnd == String::npos) {
            return {};
        }

        return element.substr(posValueStart, posValueEnd - posValueStart);
    }

    String FindElement(const String& data, const String& elementName, const pair<String, String>& elementAttributeValuePairs /* = {} */, const size_t offset /* = 0 */) {
        return {};
    }
}
