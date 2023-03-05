#include "pch.h"
#include "Scraper/Scraper.h"
#include "Parser/Parser.h"
#include "Shared/json.hpp"

#include "Scraper/Progress/InternetStatus.h"
#include "Scraper/Progress/BindStatus.h"

#include "Utility/Stopwatch.h"

using namespace Scraper;
using namespace Parser;
using namespace Progress;

using namespace nlohmann;

constexpr auto URL_CATALOG = TEXT("https://boards.4channel.org/g/catalog");
constexpr auto URL_THREAD_BASE = TEXT("https://boards.4channel.org/g/thread/");

int main() {
	Stopwatch sw(TEXT("Test"), true);

	WebScraper scraper;

	String html;
	bool done = false;

	InternetStatus callbackScraper;
	WebScraper::Callback callbackURL = [&](const auto succeeded) {
		if (!succeeded)
		{
			exit(EXIT_FAILURE);
		}

		HTMLParser parser(html);

		// hardcoded
		auto scriptPopulateAsString = parser.GetScriptAsString(4);
		auto jsonCatalogAsString = StringUtil::RemoveStringLead(scriptPopulateAsString, TEXT("var catalog = "));
		jsonCatalogAsString = StringUtil::RemoveStringTrail(jsonCatalogAsString, TEXT(";var style_group ="));

		vector<String> threadHrefs;

		auto jsonCatalog = json::parse(ToStringType<char>(jsonCatalogAsString));
		const auto& jsonCatalogItemsIterator = jsonCatalog["threads"].items();
		for (auto jsonCatalogItem = std::next(jsonCatalogItemsIterator.begin()); jsonCatalogItem != jsonCatalogItemsIterator.end(); jsonCatalogItem++)
		{
			threadHrefs.push_back(URL_THREAD_BASE + ToStringType<wchar_t>(jsonCatalogItem.key()));
		}
		threadHrefs.erase(threadHrefs.begin());

		done = true;
	};

	scraper.URLToStringAsync(URL_CATALOG, html, &callbackScraper, &callbackURL);

	while (!done) this_thread::sleep_for(1000s);

	TRACE(sw.GetTimeElapsed(TEXT("Test")).count() << TEXT("ns"));

	return 0;
}