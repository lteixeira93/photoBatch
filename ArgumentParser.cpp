#include "ArgumentParser.h"
#include "Utils.h"
#include <iostream>
#include <cctype>

void ArgumentParser::Parse(int argc, char* argv[])
{
	if (argc > 1 && argv != nullptr)
	{
		for (size_t i = 1; i < argc; i++)
		{
			std::string arg = argv[i];

			if (arg.length() >= 3)
			{
				/* Check for valid prefix containing -- */
				if (arg[0] == '-' && arg[1] == '-')
				{
					/* Removing -- */
					arg = arg.substr(2);

					/* Is an Option */
					if (arg.find_first_of('=') != std::string::npos)
					{
						const size_t equalSignPos = arg.find('=');
						if (equalSignPos != std::string::npos)
						{
							// Take string before '=' lowercase it and make key
							std::string optionName = Utils::ToLower(arg.substr(0, equalSignPos));
							// Take string after  '=' and make value
							std::string optionValue = arg.substr(equalSignPos + 1);

							auto optionIt = m_Options.find(optionName);
							if (optionIt != m_Options.end())
							{
								/* Registered option found */
								optionIt->second = optionValue;
							}
						}
					}
					/* Is a Flag */
					else
					{
						Utils::ToLower(arg);
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
	if (!flag.empty() && !Utils::HasWhiteSpaces(flag))
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
	if (!option.empty() && !Utils::HasWhiteSpaces(option))
	{
		m_Options[option] = "";
	}
}

bool ArgumentParser::IsFlagRegistered(const std::string& flag) const
{
	if (!flag.empty())
	{
		return m_Flags.count(flag) == 1;
	}

	return false;
}

bool ArgumentParser::IsOptionRegistered(const std::string& option) const
{
	if (!option.empty())
	{
		return m_Options.count(option) == 1;
	}

	return false;
}

bool ArgumentParser::IsFlagRegisteredAndParsed(const std::string& flag) const
{
	if (!flag.empty())
	{
		auto flagIt = m_Flags.find(flag);
		if (flagIt != m_Flags.end())
		{
			flagIt->second;
		}
	}

	return false;
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

void ArgumentParser::SetHelpMessage(const std::string& help)
{
	m_Help = help;
}

void ArgumentParser::PrintHelpMessage() const
{
	std::cout << m_Help << std::endl;
}