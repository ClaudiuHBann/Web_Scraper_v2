#include "pch.h"
#include "Utility.h"

namespace Utility
{
	using namespace Shared;

	String GetPointerAsHexString(const void* p) {
		return (StringStream() << p).str();
	}
}