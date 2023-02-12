#include "ArgumentParser.hpp"
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <array>

#define NUM_MODES 4
//#define DEBUG

/*
Mode: String
	- Flags:		
		-- Rename:
			Prefix: String
			StartNumber: int
		-- Convert:
			From: String
			To: String
		-- Resize:
			Width: int
			Height: int
		-- Scale:
			Amount: float
	- Options:
		-- Folder: String
		-- Filter: String
*/

namespace Args
{
	namespace Flags
	{
		static constexpr const char* Rename  = "rename";
		static constexpr const char* Convert = "convert";
		static constexpr const char* Resize	 = "resize";
		static constexpr const char* Scale	 = "scale";
	}

	namespace Opts
	{
		static constexpr const char* Folder = "folder";
		static constexpr const char* Filter = "filter";
		static constexpr const char* Width	= "width";
		static constexpr const char* Height = "height";
		static constexpr const char* Amount = "amount";
	}
}

void ValidateArguments(const ArgumentParser& argParser)
{
	// Read flags the ArgumentParser registered
	const bool bRenameMode  = argParser.GetFlag(Args::Flags::Rename);
	const bool bConvertMode = argParser.GetFlag(Args::Flags::Convert);
	const bool bResizeMode  = argParser.GetFlag(Args::Flags::Resize);
	const bool bScaleMode   = argParser.GetFlag(Args::Flags::Scale);

	// Create array of modes
	const std::array<bool, NUM_MODES> modes = { bRenameMode, bConvertMode, bResizeMode, bScaleMode };
	const std::ptrdiff_t numActiveModes = std::count(std::begin(modes), std::end(modes), true);

	// Verify if more than one mode are active
	if (numActiveModes != 1)
	{
		throw std::invalid_argument("Only one mode can be ative");
	}

	// Verify passed option folder
	const std::string folder = argParser.GetOptionAs<std::string>(Args::Opts::Folder);
	if (folder.empty())
	{
		throw std::invalid_argument("The folder must be specified");
	}

	// Verify if folder exists
	if (!std::filesystem::exists(folder))
	{
		throw std::invalid_argument("The provided folder directory doesn't exist");
	}

	// Verify if filter is valid
	const std::string filter = argParser.GetOptionAs<std::string>(Args::Opts::Filter);
	if (!filter.empty())
	{
		const std::string invalidCharacters = ":\\/*?\"<>|";
		if (filter.find_first_of(invalidCharacters) != std::string::npos)
		{
			throw std::invalid_argument("Filter cannot contain " + invalidCharacters);
		}
	}

	// Verify Resize Mode
	if (bResizeMode)
	{
		int width {};
		int height {};

		try {
			const int width  = argParser.GetOptionAs<int>(Args::Opts::Width);
			const int height = argParser.GetOptionAs<int>(Args::Opts::Height);
		}
		catch (std::invalid_argument &exception)
		{
			throw std::invalid_argument("The value informed for width or height are not valid numbers");
		}

		/* In Resize mode the width and height must be grater than 0 */
		if (width < 0 || height < 0)
		{
			throw std::invalid_argument("Width and height must be grater than 0");
		}
		if (filter.empty())
		{
			throw std::invalid_argument("Filter cannot be empty in Resize mode");
		}
	}
	// Verify Scale Mode
	else if (bScaleMode)
	{
		float amount {0.0f};

		try
		{
			amount = argParser.GetOptionAs<float>(Args::Opts::Amount);
		}
		catch (const std::invalid_argument&)
		{
			throw std::invalid_argument("The value informed for scale amount is not a valid number");
		}

		if (amount <= 0.0f)
		{
			throw std::invalid_argument("Scale amount must be grater than 0");
		}
		if (filter.empty())
		{
			throw std::invalid_argument("Filter cannot be empty in Scale mode");
		}
	}
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "en_US");

	ArgumentParser argParser;

	/* Register Flags */
	argParser.RegisterFlag(Args::Flags::Rename);
	argParser.RegisterFlag(Args::Flags::Convert);
	argParser.RegisterFlag(Args::Flags::Resize);
	argParser.RegisterFlag(Args::Flags::Scale);

	/* Register Options */
	argParser.RegisterOption(Args::Opts::Folder);
	argParser.RegisterOption(Args::Opts::Filter);
	argParser.RegisterOption(Args::Opts::Width);
	argParser.RegisterOption(Args::Opts::Height);
	argParser.RegisterOption(Args::Opts::Amount);
	
	/* Parse argv from CLI */
	argParser.Parse(argc, argv);

	try
	{
		ValidateArguments(argParser);
	}
	catch (const std::exception& exception)
	{
		std::cerr << exception.what() << std::endl;
	}

#ifdef DEBUG
	/* Get Flags */
	std::cout << std::boolalpha << "[DEBUG] Rename: "  << argParser.GetFlag("rename")  << std::endl;
	std::cout << std::boolalpha << "[DEBUG] Convert: " << argParser.GetFlag("convert") << std::endl;
	std::cout << std::boolalpha << "[DEBUG] Resize: "  << argParser.GetFlag("resize")  << std::endl;
	std::cout << std::boolalpha << "[DEBUG] Scale: "   << argParser.GetFlag("scale")   << std::endl;

	/* Get Options */
	std::cout << "[DEBUG] Folder: " << argParser.GetOptionAs<std::string>("folder") << std::endl;
	std::cout << "[DEBUG] Amount: " << argParser.GetOptionAs<float>("amount") << std::endl;
	std::cout << "[DEBUG] Amount: " << argParser.GetOptionAs<int>("amount") << std::endl;
#endif DEBUG

	return 0;
}