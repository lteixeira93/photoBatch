#include "Utils.hpp"
#include <algorithm>

namespace Utils
{
	std::string ToLower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), 
			[](unsigned char c) { return std::tolower(c); });
		return str;
	}
}
