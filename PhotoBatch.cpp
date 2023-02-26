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
	argParser.RegisterOption(Args::Opts::To);
	
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
			std::unique_ptr<Mode> photoBatchMode = CreateMode(argParser);
			if (photoBatchMode != nullptr)
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

	return 0;
}
