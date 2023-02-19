#include "ArgumentParser.h"
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <array>

#define NUM_MODES 5
//#define UTEST
//#define DEBUG

namespace Args
{
	namespace Flags
	{
		static constexpr const char* Rename		 = "rename";
		static constexpr const char* Convert	 = "convert";
		static constexpr const char* Resize		 = "resize";
		static constexpr const char* Scale		 = "scale";
		static constexpr const char* Help		 = "help";
	}

	namespace Opts
	{
		static constexpr const char* Folder		 = "folder";
		static constexpr const char* Filter		 = "filter";
		static constexpr const char* Width		 = "width";
		static constexpr const char* Height		 = "height";
		static constexpr const char* Amount		 = "amount";
		static constexpr const char* Prefix		 = "prefix";
		static constexpr const char* StartNumber = "startnumber";
		static constexpr const char* From		 = "from";
		static constexpr const char* To			 = "to";
	}
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

void ValidateArguments(const ArgumentParser& argParser)
{
	// Read flags the ArgumentParser registered
	const bool bRenameMode  = argParser.GetFlag(Args::Flags::Rename);
	const bool bConvertMode = argParser.GetFlag(Args::Flags::Convert);
	const bool bResizeMode  = argParser.GetFlag(Args::Flags::Resize);
	const bool bScaleMode   = argParser.GetFlag(Args::Flags::Scale);
	const bool bHelpMode    = argParser.GetFlag(Args::Flags::Help);

	// Create array of modes
	const std::array<bool, NUM_MODES> modes = { bRenameMode, bConvertMode, bResizeMode, bScaleMode, bHelpMode };
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
	// Verify Rename Mode
	else if (bRenameMode)
	{
		int startNumber { -1 };

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
	// Verify Help Mode
	else if (bHelpMode)
	{
		std::cout << "PhotoBatch --help" << std::endl;
		std::cout << "Commands:" << std::endl;
		std::cout << "\tFlags:" << std::endl;
		std::cout << "\t\tPhotoBatch --rename" << std::endl;
		std::cout << "\t\t\tPhotoBatch --prefix=\t\tPrefix image name to be renamed" << std::endl;
		std::cout << "\t\t\tPhotoBatch --startnumber=\tFrom which file start, must be grater than 0" << std::endl;

		std::cout << "\t\tPhotoBatch --convert" << std::endl;
		std::cout << "\t\t\tPhotoBatch --from=\t\tThe input format" << std::endl;
		std::cout << "\t\t\tPhotoBatch --to=\t\tThe output format" << std::endl;

		std::cout << "\t\tPhotoBatch --resize:" << std::endl;
		std::cout << "\t\t\tPhotoBatch --width=\t\tThe width of the image, must be grater than 0" << std::endl;
		std::cout << "\t\t\tPhotoBatch --height=\t\tThe height of the image, must be grater than 0" << std::endl;

		std::cout << "\t\tPhotoBatch --scale:" << std::endl;
		std::cout << "\t\t\tPhotoBatch --amount=\t\tAmount to be scaled, must be grater than 0" << std::endl;

		std::cout << "\tOptions:" << std::endl;
		std::cout << "\t\tPhotoBatch --folder=<path>\t\t\tExisting image folder" << std::endl;
		std::cout << "\t\tPhotoBatch --filter=\t\t\tFilter by the name" << std::endl;
		std::cout << "\t\tPhotoBatch --help\t\t\tDisplays this help message" << std::endl;
	}

}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "en_US");

	ArgumentParser argParser;

#ifndef UTEST
	/* Register Flags */
	argParser.RegisterFlag(Args::Flags::Rename);
	argParser.RegisterFlag(Args::Flags::Convert);
	argParser.RegisterFlag(Args::Flags::Resize);
	argParser.RegisterFlag(Args::Flags::Scale);
	argParser.RegisterFlag(Args::Flags::Help);
	
	/* Register Options */
	argParser.RegisterOption(Args::Opts::Folder);
	argParser.RegisterOption(Args::Opts::Filter);
	argParser.RegisterOption(Args::Opts::Width);
	argParser.RegisterOption(Args::Opts::Height);
	argParser.RegisterOption(Args::Opts::Amount);
	argParser.RegisterOption(Args::Opts::Prefix);
	argParser.RegisterOption(Args::Opts::StartNumber);
	argParser.RegisterOption(Args::Opts::From);
	argParser.RegisterOption(Args::Opts::To);
#else UTEST
	argParser.RegisterFlag("My Flag");
#endif // UTEST
	
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
	std::cout << std::boolalpha << "[DEBUG] Rename: "  << argParser.GetFlag(Args::Flags::Rename)  << std::endl;
	std::cout << std::boolalpha << "[DEBUG] Convert: " << argParser.GetFlag(Args::Flags::Convert) << std::endl;
	std::cout << std::boolalpha << "[DEBUG] Resize: "  << argParser.GetFlag(Args::Flags::Resize)  << std::endl;
	std::cout << std::boolalpha << "[DEBUG] Scale: "   << argParser.GetFlag(Args::Flags::Scale)   << std::endl;
	std::cout << std::boolalpha << "[DEBUG] Help: " << argParser.GetFlag(Args::Flags::Help) << std::endl;

	/* Get Options */
	std::cout << "[DEBUG] Folder: " << argParser.GetOptionAs<std::string>("folder") << std::endl;
	std::cout << "[DEBUG] Amount: " << argParser.GetOptionAs<float>("amount") << std::endl;
	std::cout << "[DEBUG] Amount: " << argParser.GetOptionAs<int>("amount") << std::endl;
#endif // DEBUG

	return 0;
}