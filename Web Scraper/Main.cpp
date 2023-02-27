#include "pch.h"
#include "Scraper/Scraper.h"
#include "Parser/Parser.h"

using namespace Scraper;
using namespace Parser;

int main() {
	WebScraper scraper;

	String urlBase(TEXT("https://github.com/ClaudiuHBann"));
	String url(urlBase + TEXT("?tab=repositories"));
	String html;
	if (!scraper.URLToString(url, html)) {
		return 1;
	}

	HTMLParser parser(html);

	vector<String> repositoriesURLs;
	for (auto& element : parser.GetElementsByAttributes({ { TEXT("itemprop"), TEXT("name codeRepository") } }))
	{
		auto href = parser.GetElementAttributeValueByName(element, TEXT("href"));
		href = StringUtil::RemoveStringLead(href, TEXT("about:/ClaudiuHBann"));
		repositoriesURLs.push_back(urlBase + href);
	}

	return 0;
}