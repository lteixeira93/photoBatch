#pragma once
#include <string>
#include <memory>
#include <vector>
#include <filesystem>

class Mode
{
public:
	Mode(const std::string& filter, const std::string& folder);
	
	const std::string& GetFilter() const;
	const std::string& GetFolder() const;

	/* Defining GetModeName as pure virtual function */
	virtual const std::string& GetModeName() const = 0;

	void Run();

protected:
	virtual void RunImpl() = 0;
	std::vector<std::filesystem::path> GetFiles(const std::filesystem::path& extension = "") const;

private:
	std::string m_Filter;
	std::string m_Folder;
};

namespace Args
{
	namespace Flags
	{
		static constexpr const char* Rename  = "rename";
		static constexpr const char* Convert = "convert";
		static constexpr const char* Resize  = "resize";
		static constexpr const char* Scale   = "scale";
		static constexpr const char* Help    = "help";
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

/* Forward declaration, no need to include .h file (Faster compilation) */
class ArgumentParser;

std::unique_ptr<Mode> CreateMode(const ArgumentParser& argParser);


