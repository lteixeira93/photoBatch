#include "ArgumentParser.hpp"
#include "Utils.hpp"

void ArgumentParser::Parse(int argc, char* argv[])
{
	if (argc > 1)
	{
		for (size_t i = 1; i < argc; i++)
		{
			std::string arg = argv[i];
			Utils::ToLower(arg);

			if (arg.length() >= 3)
			{
				/* Check for valid prefix containing -- */
				if (arg[0] == '-' && arg[1] == '-')
				{
					/* Removing -- */
					arg = arg.substr(2);

					/* Option */
					if (arg.find_first_of('=') != std::string::npos)
					{
						const size_t equalSignPos = arg.find('=');
						if (equalSignPos != std::string::npos)
						{
							// Take string before = and make key
							std::string optionName = arg.substr(0, equalSignPos);
							// Take string after  = and make value
							std::string optionValue = arg.substr(equalSignPos + 1);

							auto optionIt = m_Options.find(optionName);
							if (optionIt != m_Options.end())
							{
								/* Registered option found */
								optionIt->second = optionValue;
							}

						}
					}
					/* Flag */
					else
					{
						auto flagIt = m_Flags.find(arg);
						if (flagIt != m_Flags.end())
						{
							flagIt->second = true;
						}
					}
				}
			}
		}

	}
}

void ArgumentParser::RegisterFlag(const std::string& flag)
{
	if (!flag.empty())
	{
		m_Flags.insert(std::make_pair(flag, false));
	}
}

bool ArgumentParser::GetFlag(const std::string& flag) const
{
	if (!flag.empty())
	{
		auto flagIt = m_Flags.find(flag);
		if (flagIt != m_Flags.end())
		{
			return flagIt->second;
		}
	}
	return false;
}

void ArgumentParser::RegisterOption(const std::string& option)
{
	if (!option.empty())
	{
		m_Options[option] = "";
	}
}

std::string ArgumentParser::GetOption(const std::string& option) const
{
	if (!option.empty())
	{
		auto optionIt = m_Options.find(option);
		if (optionIt != m_Options.end())
		{
			return optionIt->second;
		}
	}

	static std::string EmptyOption = "";
	return EmptyOption;
}

float ArgumentParser::GetOptionAsFloat(const std::string& option) const
{
	const std::string& optionValue = GetOption(option);
	if (!optionValue.empty())
	{
		return std::stof(optionValue);
	}

	return -1;
}

int ArgumentParser::GetOptionAsInt(const std::string& option) const
{
	const std::string& optionValue = GetOption(option);
	if (!optionValue.empty())
	{
		return std::stoi(optionValue);
	}

	return -1;
}