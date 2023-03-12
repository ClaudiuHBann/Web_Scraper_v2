#pragma once

#include "String.h"

namespace Shared {
	constexpr auto FILE_NAME_ILLEGAL_CHARS = TEXT(R"(<>:"/\|?*)");

	class File {
	public:
		File(const String& file);

		static bool Exists(const String& file);

		static String ReadAllText(const String& file);

		static String MakeFileNameValid(const String& fileName);
		static bool IsFileNameValid(const String& fileName);

		String GetFileExtension();

	private:
		String mFile;
	};
}
