#include "ConvertMode.h"
#include <iostream>
#include <stb_image.h>
#include <stb_image_write.h>

std::string ToString(ConvertMode::Format format)
{
	switch (format)
	{
	case ConvertMode::Format::JPG:
		return "jpg";
	case ConvertMode::Format::PNG:
		return "png";
	default:
		break;
	}

	return "";
}

std::ostream& operator<<(std::ostream& os, const ConvertMode::Format format)
{
	os << ToString(format);
	return os;
}

ConvertMode::ConvertMode(const std::string filter, const std::string folder, Format fromFormat, Format toFormat)
	: Mode{ filter, folder }, m_FromFormat{ fromFormat }, m_ToFormat{ toFormat }
{}

const std::string& ConvertMode::GetModeName() const
{
	static const std::string ConvertModeName = "[Convert]: ";
	return ConvertModeName;
}

void ConvertMode::RunImpl()
{
	std::cout << GetModeName() << "Mode        : Convert" << std::endl;
	std::cout << GetModeName() << "Folder      : " << GetFolder() << std::endl;
	std::cout << GetModeName() << "Filter      : " << GetFilter() << std::endl;
	std::cout << GetModeName() << "From        : " << m_FromFormat << std::endl;
	std::cout << GetModeName() << "To	       : " << m_ToFormat << std::endl;

	const std::filesystem::path fromExtension = "." + ToString(m_FromFormat);
	for (const std::filesystem::path& filepath : GetFiles(fromExtension))
	{
		std::cout << GetModeName() << "Converting: " << filepath << std::endl;

		int width{0};
		int height{0};

		/* Number of components part of the image */
		int numComp{0};
		const int numReqComp{ 3 };

		/* Each pixel contain RGB and each pixel goes to 0 - 255 */
		unsigned char* data = stbi_load(filepath.string().c_str(), &width, &height, &numComp, numReqComp);
		int writeResult{0};

		if(data != nullptr)
		{
			std::filesystem::path destFilePath = filepath;
			destFilePath.replace_extension(ToString(m_ToFormat));

			/* Using c_str - C implemented API */
			switch (m_ToFormat)
			{
			case ConvertMode::Format::PNG:
				writeResult = stbi_write_png(destFilePath.string().c_str(), width, height, numComp, data, 0);
				break;
			case ConvertMode::Format::JPG:
				writeResult = stbi_write_jpg(destFilePath.string().c_str(), width, height, numComp, data, 50);
				break;
			default:
				break;
			}

			if (writeResult == 0)
			{
				std::cout << GetModeName() << "Error converting file " << filepath << std::endl;
			}

			stbi_image_free(data);
		}
		else
		{
			std::cout << GetModeName() << "Error loading filepath " << filepath << std::endl;
		}
	}
		
}