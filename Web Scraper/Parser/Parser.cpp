#include "pch.h"
#include "Parser.h"

namespace Parser {
    constexpr ::TCHAR QUOTE_CHAR = TEXT('"');
    constexpr size_t QUOTE_CHAR_SIZE = 1;

    constexpr ::TCHAR TAG_OPEN_START = TEXT('<');
    constexpr size_t TAG_OPEN_START_SIZE = 1;
    constexpr ::TCHAR TAG_CLOSE_START[] = TEXT("</");
    constexpr ::TCHAR TAG_OPEN_END = TEXT('>');
    constexpr ::TCHAR TAG_OPEN_END_SPECIAL[] = TEXT("/>");
    constexpr size_t TAG_OPEN_END_SPECIAL_SIZE = 2;
    constexpr ::TCHAR TAG_CLOSE_END = TEXT('>');

    constexpr ::TCHAR BLANK_CHARS[] = TEXT(" \t\r\n");
    const vector<String> TAGS_END { TAG_OPEN_END, TAG_OPEN_END_SPECIAL };

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
        /*if (!IsTagSelfClosed(elementName)) {
            return {};
        }*/

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

    inline String FindElementName(const String& data, const size_t offset = 0) {

    }

    String FindElement(const String& data, const String& elementName, const pair<String, String>& elementAttributeValuePairs /* = {} */, const size_t offset /* = 0 */) {
        return {};
    }

    inline bool IsTagSelfClosed(const wstring& elementName) {
        return ranges::find(TAGS_SELF_CLOSED, elementName) != TAGS_SELF_CLOSED.end();
    }

    inline pair<wstring, size_t> GetClosest(const wstring& data, const list<wstring>& elements, const size_t offset = 0) {
        auto it = std::min_element(elements.begin(), elements.end(), [&] (const auto& left, const auto& right) { return data.find(left, offset) < data.find(right, offset); });
        return { *it, data.find(*it, offset) };
    }

    inline wstring TrimBlankChars(const wstring& data, const size_t offset = 0) {
        auto dataStart = data.find_first_not_of(BLANK_CHARS, offset);
        auto dataEnd = data.find_last_not_of(BLANK_CHARS, dataStart + 1);
        return data.substr(dataStart, dataEnd - dataStart);
    }
}
