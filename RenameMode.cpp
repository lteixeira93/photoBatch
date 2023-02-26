#include "RenameMode.h"
#include <iostream>
#include <filesystem>
#include <vector>

RenameMode::RenameMode(const std::string& filter, const std::string& folder, const std::string& prefix, int startNumber)
	: Mode{ filter, folder }, m_Prefix{ prefix }, m_StartNumber{ startNumber }
{}

const std::string& RenameMode::GetModeName() const
{
	static const std::string RenameModeName = "[Rename]: ";
	return RenameModeName;
}

void RenameMode::RunImpl()
{
	std::cout << GetModeName() << "Mode        : Rename" << std::endl;
	std::cout << GetModeName() << "Folder      : " << GetFolder() << std::endl;
	std::cout << GetModeName() << "Filter      : " << GetFilter() << std::endl;
	std::cout << GetModeName() << "Prefix      : " << m_Prefix << std::endl;
	std::cout << GetModeName() << "StartNumber : " << m_StartNumber << std::endl;

	int currentNumber = m_StartNumber;
	for (const std::filesystem::path& filepath : GetFiles())
	{
		/* Defining rename convention as prefix+startNumber+extension*/
		const std::filesystem::path extension = filepath.extension();
		const std::string newFileName = m_Prefix + std::to_string(currentNumber) + extension.string();

		std::filesystem::path newFilePath = filepath;
		newFilePath.replace_filename(newFileName);

		try
		{
			/* Renaming as prefix+startNumber+extension*/
			std::filesystem::rename(filepath, newFilePath);
			std::cout << GetModeName() << filepath << " -> " << newFilePath << std::endl;
		}
		catch (const std::exception& exception)
		{
			std::cerr << GetModeName() << "Error renaming \"" << filepath << "\": " << exception.what() << std::endl;
		}


		currentNumber++;
	}

}
