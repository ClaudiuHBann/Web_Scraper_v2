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
    String url(TEXT("https://github.com/ClaudiuHBann?tab=repositories"));
    string str;
    if (URLToString(url, str)) {
        TRACE(TEXT('\n') << ToStringType<wchar_t>(str));
    }

    return 0;
}