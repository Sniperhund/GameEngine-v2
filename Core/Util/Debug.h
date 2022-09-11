#pragma once

#include <iostream>

#ifdef _DEBUG
#define DebugLog(msg) std::cout << __FILE__ << " " << __LINE__ << ": " << msg << std::endl
#define DebugError(msg) std::cout << __FILE__ << " " << __LINE__ << ": " << msg << std::endl; \
	__debugbreak()
#else
#define DebugLog(msg)
#define DebugError(msg)
#endif
