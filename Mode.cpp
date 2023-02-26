#include "Mode.h"
#include "RenameMode.h"
#include "ConvertMode.h"
#include "ResizeMode.h"
#include "ResizeMode.h"
#include "ScaleMode.h"
#include "ArgumentParser.h"
#include "Utils.h"
#include <array>
#include <filesystem>
#include <iostream>
#include <chrono>

#define NUM_MODES 4

Mode::Mode(const std::string& filter, const std::string& folder)
	: m_Filter { filter }, m_Folder { folder }
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
	/* Run implementation and measure how long the operation takes to execute */
	//std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
	using ClockT = std::chrono::high_resolution_clock;
	
	ClockT::time_point startTime = ClockT::now();	
	RunImpl();
	ClockT::time_point endTime = ClockT::now();

	ClockT::duration elapsedTime = endTime - startTime; // In nanoseconds
	std::chrono::milliseconds elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime);

	std::cout << GetModeName() << "Operation finished in " << elapsedTimeMs.count() << " ms" << std::endl;
}

std::vector<std::filesystem::path> Mode::GetFiles(const std::filesystem::path& extension) const
{
	std::vector<std::filesystem::path> files;
	int numSkippedFiles{ 0 };

	/* Colect all files that correspond to the specified filter */
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(GetFolder()))
	{
		const bool bIsFile = std::filesystem::is_regular_file(entry.path());
		const bool bMatchFilter = GetFilter().empty() || (entry.path().string().find(GetFilter()) != std::string::npos);
		const bool bMatchExtension = extension.empty() || (entry.path().extension() == extension);

		if (bIsFile && bMatchFilter &&bMatchExtension)
		{
			files.push_back(entry.path());
		}
		else
		{
			numSkippedFiles++;
		}

	}

	std::cout << GetModeName() << "Number of founded files: " << files.size() << std::endl;
	std::cout << GetModeName() << "Number of ignored files: " << numSkippedFiles << std::endl;

	return files;
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
	if (filter.empty() || Utils::HasInvalidChars(filter))
	{
		throw std::invalid_argument("The filter cannot contain " + Utils::GetInvalidChars());
	}

	// Verify Resize Mode
	if (bResizeMode)
	{
		int width{0};
		int height{0};

		try {
			width = argParser.GetOptionAs<int>(Args::Opts::Width);
			height = argParser.GetOptionAs<int>(Args::Opts::Height);
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

		return std::make_unique<ResizeMode>(filter, folder, width, height);
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

		return std::make_unique<ScaleMode>(filter, folder, amount);
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
		if (prefix.empty() || Utils::HasInvalidChars(prefix))
		{
			throw std::invalid_argument("The prefix cannot be in empty or contain " + Utils::GetInvalidChars());
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

		const std::map<std::string, ConvertMode::Format> convertOptionsMap = {
			{ "jpg" , ConvertMode::Format::JPG },
			{ "png" , ConvertMode::Format::PNG }
		};

		return std::make_unique<ConvertMode>(filter, folder, convertOptionsMap.at(from), convertOptionsMap.at(to));
	}

	return nullptr;
}
