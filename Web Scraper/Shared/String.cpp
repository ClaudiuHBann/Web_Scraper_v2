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
			return pos != String::npos ? data.substr(0, pos) : data;
		}

		String StringTrail(const String& data, const String& trail) {
			auto pos = data.rfind(trail);
			return pos != String::npos ? data.substr(pos + trail.size()) : String();
		}

		bool Contains(const String& data, const String& query) {
			return data.find(query) != String::npos;
		}

		/*vector<String> Split(const String& data, const String& delimiter) {
			auto dataSplitted = data | views::split(delimiter);
			return vector<String>(dataSplitted.begin(), dataSplitted.end());
		}*/

		vector<size_t> FindAllOccurrences(const String& data, const String& query) {
			vector<size_t> occurrences;

			auto pos = data.find(query);
			while (pos != String::npos) {
				occurrences.push_back(pos);
				pos = data.find(query, pos + query.size());
			}

			return occurrences;
		}
	}
}