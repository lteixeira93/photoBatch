#include "ScaleMode.h"
#include <iostream>
#include <stb_image.h>

ScaleMode::ScaleMode(const std::string& filter, const std::string& folder, float scaleFactor)
	: ResizeMode { filter, folder }, m_ScaleFactor { scaleFactor }
{}

const std::string& ScaleMode::GetModeName() const
{
	static const std::string ScaleModeName = "[Scale]: ";
	return ScaleModeName;
}

void ScaleMode::RunImpl()
{
	std::cout << GetModeName() << "Mode        : Scale" << std::endl;
	std::cout << GetModeName() << "Folder      : " << GetFolder() << std::endl;
	std::cout << GetModeName() << "Filter      : " << GetFilter() << std::endl;
	std::cout << GetModeName() << "Factor      : " << m_ScaleFactor << std::endl;

	for (const std::filesystem::path& filepath : GetFiles())
	{
		std::cout << GetModeName() << "Scaling: " << filepath << std::endl;

		/* Read image */
		int width{ 0 };
		int height{ 0 };
		int NumComp{ 0 };

		/* Get image size */
		stbi_info(filepath.string().c_str(), &width, &height, &NumComp);

		const int newWidth  = static_cast<int> (width * m_ScaleFactor);
		const int newHeight = static_cast<int> (height * m_ScaleFactor);

		ResizeImage(filepath, newWidth, newHeight);
	}
}
