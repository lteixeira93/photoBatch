#include "ResizeMode.h"
#include <iostream>
#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_image_resize.h>

ResizeMode::ResizeMode(const std::string& filter, const std::string& folder, int width, int height)
	: Mode { filter, folder }, m_Width { width }, m_Height { height }
{}

const std::string& ResizeMode::GetModeName() const
{
	static const std::string ResizeModeName = "[Resize]: ";
	return ResizeModeName;
}

void ResizeMode::ResizeImage(const std::filesystem::path filepath, int newWidth, int newHeight) const
{
	/* Read image */
	int inputWidth { 0 };
	int inputHeight{ 0 };
	int inputNumComp{ 0 };
	const int numCompReq{ 4 }; // 3 for RGB and 1 for transparency in case of .png files

	/* Each pixel contain RGB and each pixel goes to 0 - 255 */
	unsigned char* inputData = stbi_load(filepath.string().c_str(), &inputWidth, &inputHeight, &inputNumComp, numCompReq);
	int writeResult{ 1 };

	if (inputData != nullptr)
	{
		const size_t numOutputPixels = newWidth * newHeight * numCompReq;
		std::vector<uint8_t> outputData(numOutputPixels, 0);
		
		/* Resize image in memory */
		auto resize_result = stbir_resize_uint8(inputData, inputWidth, inputHeight, 0, 
			outputData.data(), newWidth, newHeight, 0, numCompReq);

		if (resize_result == 1)
		{
			/* Write image */
			int writeResult{ 0 };
			const std::filesystem::path extension = filepath.extension();
			if (extension == ".jpg")
			{
				writeResult = stbi_write_jpg(filepath.string().c_str(), newWidth, newHeight, numCompReq, outputData.data(), 50);
			}
			else if (extension == ".png")
			{
				writeResult = stbi_write_png(filepath.string().c_str(), newWidth, newHeight, numCompReq, outputData.data(), 0);
			}
			else
			{ 
				std::cout << GetModeName() << "Format not supported " << filepath << std::endl;
			}
			if (writeResult == 0)
			{
				std::cout << GetModeName() << "Error writing " << filepath << std::endl;
			}
		}
		else
		{
			std::cout << GetModeName() << "Error resizing filepath " << filepath << std::endl;
		}

		stbi_image_free(inputData);
	}
	else
	{
		std::cout << GetModeName() << "Error loading filepath " << filepath << std::endl;
	}
}

void ResizeMode::RunImpl()
{
	std::cout << GetModeName() << "Mode        : Resize" << std::endl;
	std::cout << GetModeName() << "Folder      : " << GetFolder() << std::endl;
	std::cout << GetModeName() << "Filter      : " << GetFilter() << std::endl;
	std::cout << GetModeName() << "Width       : " << m_Width << std::endl;
	std::cout << GetModeName() << "Height      : " << m_Height << std::endl;

	if (m_Width > 0 && m_Height > 0)
	{
		for (const std::filesystem::path& filepath : GetFiles())
		{
			std::cout << GetModeName() << "Resizing " << filepath << std::endl;
			ResizeImage(filepath, m_Width, m_Height);
		}
	}
}
