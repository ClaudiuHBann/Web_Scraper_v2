#include "pch.h"
#include "Parser.h"

namespace Parser {
    constexpr TCHAR QUOTE_CHAR = TEXT('"');
    constexpr size_t QUOTE_CHAR_SIZE = 1;

    vector<size_t> FindAllOccurrences(const String& data, const String& query) {
        vector<size_t> occurrences;

        auto pos = data.find(query);
        while (pos != String::npos) {
            occurrences.push_back(pos);
            pos = data.find(query, pos + query.size());
        }

        return occurrences;
    }

    String GetAttributeValue(const String& element, const String& attribute, const size_t offset /* = 0 */) {
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

    vector<String> Split(const String& data, const String& delimiter) {
        auto dataSplitted = data | views::split(delimiter);
        return vector<String>(dataSplitted.begin(), dataSplitted.end());
    }
}
