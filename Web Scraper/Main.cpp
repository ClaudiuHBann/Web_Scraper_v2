#include "pch.h"

#include "Scraper/Scraper.h"
#include "Parser/Parser.h"
#include "Shared/json.hpp"

#include "Scraper/Progress/InternetStatus.h"
#include "Scraper/Progress/BindStatus.h"

#include "Utility/Stopwatch.h"
#include "Utility/GUID.h"
#include "Shared/File.h"

using namespace Scraper;
using namespace Parser;
using namespace Progress;

using namespace nlohmann;

constexpr auto URL_4CHAN_CATALOG = TEXT("https://boards.4channel.org/g/catalog");
constexpr auto URL_4CHAN_THREAD_BASE = TEXT("https://boards.4channel.org/g/thread/");
String DIRECTORY_4CHAN_BASE;

void GetJSONCatalog(const String& script, vector<String>& threadHrefs, vector<String>& threadNames) {
	auto jsonCatalogAsString = StringUtil::RemoveStringLead(script, TEXT("var catalog = "));
	jsonCatalogAsString = StringUtil::RemoveStringTrail(jsonCatalogAsString, TEXT(";var style_group ="));

	auto jsonCatalog = json::parse(ToStringType<char>(jsonCatalogAsString));
	const auto& jsonCatalogItemsIterator = jsonCatalog["threads"].items();
	for (auto jsonCatalogItem = std::next(jsonCatalogItemsIterator.begin());
		 jsonCatalogItem != jsonCatalogItemsIterator.end(); jsonCatalogItem++) {
		threadHrefs.push_back(URL_4CHAN_THREAD_BASE + ToStringType<wchar_t>(jsonCatalogItem.key()));
		threadNames.push_back(ToStringType<wchar_t>(jsonCatalogItem.value()["sub"].dump()));
	}
}

void GetSourcesAndFiles(const String& html, const String& hrefStr, const String& nameStr, atomic<int>& filesTotal, vector<String>& srcs, vector<String>& files) {
	HTMLParser parser;
	parser.Parse(html);

	auto id = TEXT("t") + StringUtil::StringTrail(hrefStr, TEXT("/"));
	auto threadElement = parser.GetElementById(id);

	HTMLParser parserThread;
	parserThread.Parse(HTMLParser::GetElementInnerHTML(threadElement));

	auto threadAs = parserThread.GetElementsByClassName(TEXT("fileThumb"));
	Utility::GUID guid;

	long length {};
	threadAs->get_length(&length);
	filesTotal += (int)length;
	for (long i = 0; i < length; i++) {
		auto threadA = HTMLParser::GetElementFromCollectionByIndex(threadAs, i);

		auto href = HTMLParser::GetElementAttributeValueByName(threadA, TEXT("href"));
		auto hrefFull = TEXT("https://") + StringUtil::RemoveStringLead(href, TEXT("//"));
		srcs.push_back(hrefFull);

		auto fileName = Utility::GUID().GetString() + TEXT(".") + File(hrefFull).GetFileExtension();
		auto file = DIRECTORY_4CHAN_BASE;
		if (File::IsFileNameValid(nameStr)) {
			file += File::MakeFileNameValid(nameStr);
		} else {
			file += guid.GetString();
		}
		file += TEXT(R"(\)") + fileName;
		files.push_back(file);
	}
}

int _tmain(int, wchar_t** argv) {
	DIRECTORY_4CHAN_BASE = StringUtil::RemoveStringTrail(argv[0], TEXT(R"(\)")) + TEXT(R"(\4chan\)");

	Stopwatch sw(TEXT("Test"), true);

	WebScraper scraper;

	String htmlCatalog;
	atomic filesCurrent = 0, filesTotal = 0;

	InternetStatus callbackScraper;
	BindStatus bindStatus;
	WebScraper::CallbackRaw callbackURL = [&] (const auto& html) {
		HTMLParser parser;
		parser.Parse(html);

		vector<String> threadHrefs, threadNames;
		GetJSONCatalog(parser.GetScriptAsString(4), threadHrefs, threadNames);

		for (auto href = threadHrefs.begin(), name = threadNames.begin(); href != threadHrefs.end() && name != threadNames.end(); href++, name++) {
			thread([&, hrefStr = *href, nameStr = *name] () {
				auto callbackHref = make_shared<WebScraper::CallbackRaw>([&, hrefStr, nameStr] (const auto& htmlHref) {
					   auto callbackFile = make_shared<WebScraper::CallbackRaw>([&] (const auto&) { filesCurrent++; });

			vector<String> srcs, files;
			GetSourcesAndFiles(htmlHref, hrefStr, nameStr, filesTotal, srcs, files);
			for (auto src = srcs.begin(), file = files.begin(); src != srcs.end() && file != files.end(); src++, file++) {
				scraper.URLToFileAsync(*src, *file, &bindStatus, &callbackFile);
			}
																		 });
			scraper.URLToStringAsync(hrefStr, &callbackScraper, &callbackHref);
				   }).detach();
		}
	};

	scraper.URLToStringAsync(URL_4CHAN_CATALOG, &callbackScraper, &callbackURL);

	this_thread::sleep_for(10s);
	while (filesCurrent != filesTotal) this_thread::sleep_for(1ms);

	TRACE(duration_cast<seconds>(sw.GetTimeElapsed(TEXT("Test"))));

	return 0;
}