//#include "pch.h"
//#include "Scraper/Scraper.h"
//#include "Shared/File.h"
//#include "Utility/Logger.h"
//#include "Parser/Parser.h"
//
//using namespace Scraper;
//using namespace Shared;
//using namespace Utility;
//using namespace Parser;
//
//int main() {
//    String html(TEXT(R"(
//<!DOCTYPE html>
//<html>
//<head>
//   <title>Self-closing tags</title>
//</head>
// <body>
//	<img src="https://cdn.pixabay.com/photo/2021/09/15/11/44/insect-6626635__340.jpg" alt="insect-bee"/>
//  </body>
//</html>
//)"));
//
//    TRACE(FindElementSpecial(html, TEXT("img")));
//
//    /*String url(TEXT("https://example.com/"));
//    String file;
//    if (SUCCEEDED(URLToFileCache(url, file))) {
//        auto text = File::ReadAllText(file);
//        TRACE(TEXT('\n') << text);
//    }*/
//
//    return 0;
//}