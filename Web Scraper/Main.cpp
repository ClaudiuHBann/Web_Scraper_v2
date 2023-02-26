#include "pch.h"
#include "Scraper/Scraper.h"
#include "Parser/Parser.h"

using namespace Scraper;
using namespace Parser;

int main() {
	WebScraper scraper;
	InternetStatus scraperEvents;

	String url(TEXT("https://github.com/ClaudiuHBann?tab=repositories"));
	String urlHTML;
	if (!scraper.URLToString(url, urlHTML, TEXT(""), 0, &scraperEvents)) {
		return 1;
	}

	TRACE(urlHTML);
	HTMLParser parser(urlHTML);

	return 0;
}