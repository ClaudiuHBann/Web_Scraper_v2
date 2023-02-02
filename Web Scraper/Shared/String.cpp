#include "pch.h"
#include "String.h"

namespace Shared {
    namespace StringUtil {
        String RemoveStringLead(const String& data, const String& lead) {
            auto pos = data.find(lead);
            return pos != String::npos ? data.substr(pos + lead.size()) : data;
        }

        String StringLead(const String& data, const String& lead) {
            auto pos = data.find(lead);
            return pos != String::npos ? data.substr(0, pos + lead.size()) : String();
        }

        String RemoveStringTrail(const String& data, const String& trail) {
            auto pos = data.rfind(trail);
            return pos != String::npos ? data.substr(pos + trail.size()) : data;
        }

        String StringTrail(const String& data, const String& trail) {
            auto pos = data.rfind(trail);
            return pos != String::npos ? data.substr(0, pos + trail.size()) : String();
        }

        bool Contains(const String& data, const String& query) {
            return data.find(query) != String::npos;
        }
    }
}