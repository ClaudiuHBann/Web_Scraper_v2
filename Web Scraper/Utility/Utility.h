#pragma once

#include "Shared/String.h"

namespace Utility
{
	using namespace Shared;

	String GetPointerAsHexString(const void* p);

	bool IsConsolePresent();
}