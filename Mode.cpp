#include "Mode.h"
#include "RenameMode.h"
#include "ArgumentParser.h"
#include "Utils.h"
#include <array>
#include <filesystem>
#include <iostream>

#define NUM_MODES 4

Mode::Mode(const std::string& filter, const std::string& folder)
	: m_Filter{ filter }, m_Folder { folder }
{}

const std::string& Mode::GetFilter() const
{
	return m_Filter;
}

const std::string& Mode::GetFolder() const
{
	return m_Folder;
}

void Mode::Run()
{
	/* Measure how long the operation took to run */
	RunImpl();
}

static const std::string& GetInvalidChars(void)
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

/* 
Factory method - Knows how to instantiate a base class based on parameter.
Instantiate a different object based on what is passed as parameter
*/
std::unique_ptr<Mode> CreateMode(const ArgumentParser& argParser)
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
	if (!filter.empty() && HasInvalidChars(filter))
	{
		throw std::invalid_argument("The filter cannot contain " + GetInvalidChars());
	}

	// Verify Resize Mode
	if (bResizeMode)
	{
		int width{};
		int height{};

		try {
			const int width = argParser.GetOptionAs<int>(Args::Opts::Width);
			const int height = argParser.GetOptionAs<int>(Args::Opts::Height);
		}
		catch (std::invalid_argument& exception)
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
		float amount{ 0.0f };

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
	// Verify Rename Mode
	else if (bRenameMode)
	{
		int startNumber{ -1 };

		try
		{
			startNumber = argParser.GetOptionAs<int>(Args::Opts::StartNumber);
		}
		catch (const std::invalid_argument&)
		{
			throw std::invalid_argument("The value informed for start number is not a valid number");
		}

		std::string prefix = argParser.GetOptionAs<std::string>(Args::Opts::Prefix);

		if (startNumber < 0)
		{
			throw std::invalid_argument("Start number amount must be grater or equal than 0");
		}
		if (prefix.empty() || HasInvalidChars(prefix))
		{
			throw std::invalid_argument("The prefix cannot be in empty or contain " + GetInvalidChars());
		}

		return std::make_unique<RenameMode>(filter, folder, prefix, startNumber);
	}
	// Verify Convert Mode
	else if (bConvertMode)
	{
		const std::string from = argParser.GetOptionAs<std::string>(Args::Opts::From);
		const std::string to = argParser.GetOptionAs<std::string>(Args::Opts::To);

		/* Could use set for faster search since it uses OlogN where N is the number of elements */
		const std::array <std::string, 2> convertOptions = { "jpg", "png" };

		const bool bIsFromValid = std::find(std::begin(convertOptions), std::end(convertOptions), from) != std::end(convertOptions);
		const bool bIsToValid = std::find(std::begin(convertOptions), std::end(convertOptions), to) != std::end(convertOptions);

		if (!bIsFromValid || !bIsToValid)
		{
			throw std::invalid_argument("From and To must be jpg or png");
		}
		if (from == to)
		{
			throw std::invalid_argument("From and To must be different");
		}
	}

	return nullptr;
}
