#include "Utils.h"
#include <algorithm>

namespace Utils
{
	std::string ToLower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), 
			[](unsigned char c) { return std::tolower(c); });
		return str;
	}

	bool HasWhiteSpaces(const std::string& str)
	{
		return str.find(' ') != std::string::npos;
	}
}
