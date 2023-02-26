#include "pch.h"
#include "Scraper/Scraper.h"
#include "Shared/File.h"
#include "Utility/Logger.h"
#include "Parser/Parser.h"

using namespace Scraper;
using namespace Shared;
using namespace Utility;
using namespace Parser;

int main() {
	WebScraper webScraper;
	InternetStatus callback;

	String url(TEXT("https://github.com/ClaudiuHBann?tab=repositories"));
	string str;
	if (webScraper.URLToString(url, str, TEXT(""), 0, &callback)) {
		TRACE(ToStringType<wchar_t>(str));
	}

	return 0;
}