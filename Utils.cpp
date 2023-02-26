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

	const std::string& GetInvalidChars(void)
	{
		static const std::string invalidCharacters = ":\\/*?\"<>|";

		/* Returns static reference */
		return invalidCharacters;
	}

	bool HasInvalidChars(const std::string& str)
	{
		const bool bHasInvalidChars = (str.find_first_of(GetInvalidChars()) != std::string::npos);

		return bHasInvalidChars;
	}
}
