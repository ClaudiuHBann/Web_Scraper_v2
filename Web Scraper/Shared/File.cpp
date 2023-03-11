#include "pch.h"
#include "File.h"
#include "Utility/GUID.h"

namespace Shared {
	using namespace Utility;

	/* static */ bool File::Exists(const String& file) {
		return exists(file);
	}

	/* static */ String File::ReadAllText(const String& file) {
		if (!Exists(file)) {
			return {};
		}

		IfStream stream(file);
		return String((istreambuf_iterator<::TCHAR>(stream)), istreambuf_iterator<::TCHAR>());
	}

	File::File(const String& file) :mFile(file) {}

	String File::GetFileExtension() {
		return StringUtil::StringTrail(mFile, TEXT("."));
	}

	/* static */ String File::MakeFileNameValid(const String& fileName) {
		static const String FILE_NAME_ILLEGAL_CHARS_STR(FILE_NAME_ILLEGAL_CHARS);
		String fileNameNew{};

		for (const auto& c : fileName)
		{
			if (FILE_NAME_ILLEGAL_CHARS_STR.find(c) == String::npos)
			{
				fileNameNew.push_back(c);
			}
		}

		if (!fileNameNew.size())
		{
			Utility::GUID guid;
			fileNameNew = guid.GetString();
		}

		return fileNameNew;
	}
}
