#ifndef _ARGUMENT_PARSER_HPP_
#define _ARGUMENT_PARSER_HPP_

#include "Utils.hpp"
#include <string>
#include <map>

/* Class used to read arguments from CLI */
class ArgumentParser
{
public:
	/* Process argv[] */
	void Parse(int argc, char* argv[]);
	
	/* Register a flag used during the parse */
	void RegisterFlag(const std::string& flag);

	/* Register an option used during the parse */
	void RegisterOption(const std::string& option);
	
	/* Get the flag value during the parse */
	bool GetFlag(const std::string& flag) const;

	/** Template to GetOption <int>, <float>, <string> **/
	template <typename T>
	T GetOptionAs(const std::string& option) const;

	/** Specializing templates **/
	/* Return option value passed as parameter as float */
	template<>
	float GetOptionAs(const std::string& option) const { return GetOptionAsFloat(option); }

	/* Return option value passed as parameter as int */
	template<>
	int GetOptionAs(const std::string& option) const { return GetOptionAsInt(option); }

	/* Return option value passed as parameter as std::string */
	template<>
	std::string GetOptionAs(const std::string& option) const { return GetOption(option); }

private:
	/* Private maps for flags and options */
	std::map<std::string, bool> m_Flags;
	std::map<std::string, std::string> m_Options;

	/* Get the value from option during the parse */
	std::string GetOption(const std::string& option) const;
	float GetOptionAsFloat(const std::string& option) const;
	int GetOptionAsInt(const std::string& option) const;
};

#endif // _ARGUMENT_PARSER_HPP_