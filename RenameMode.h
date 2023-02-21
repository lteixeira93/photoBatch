#pragma once

#include "Mode.h"

/* final -> Last class in hierarchy */
class RenameMode final : public Mode
{
public:
	RenameMode(const std::string& filter, const std::string& folder, const std::string& prefix, int startNumber);

	const std::string& GetModeName() const override;

protected:
	void RunImpl() override;

private:
	std::string m_Prefix;
	int m_StartNumber;
};
