#include "ArgumentParser.h"
#include "Mode.h"
#include <iostream>
#include <iomanip>

//#define DEBUG

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "en_US");

	ArgumentParser argParser;

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
	
	/* Parse argv from CLI */
	argParser.Parse(argc, argv);	

	argParser.SetHelpMessage(R"(
PhotoBatch --help
Commands:
  Flags:
	PhotoBatch --rename
		PhotoBatch --prefix=<Prefix image name to be renamed>
		PhotoBatch --startnumber=<From which file start, must be grater than 0>
	PhotoBatch --convert
		PhotoBatch --from=<The input format>
		PhotoBatch --to=<The output format>
	PhotoBatch --resize:
		PhotoBatch --width=<The width of the image, must be grater than 0>
		PhotoBatch --height=<The height of the image, must be grater than 0>
	PhotoBatch --scale:
		PhotoBatch --amount=<Amount to be scaled, must be grater than 0>
	PhotoBatch --help   
		Displays this help message.
  Options:
	PhotoBatch --folder=<Existing image folder>
	PhotoBatch --filter=<Filter by the name>)"
	);

	if (argParser.GetFlag(Args::Flags::Help))
	{
		argParser.PrintHelpMessage();
	}
	else
	{
		try
		{
			if (std::unique_ptr<Mode> photoBatchMode = CreateMode(argParser))
			{
				photoBatchMode->Run();
			}
		}
		catch (const std::exception& exception)
		{
			std::cerr << exception.what() << std::endl;
			std::cerr << "Use the flag --help to know how to use the application" << std::endl;
		}
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