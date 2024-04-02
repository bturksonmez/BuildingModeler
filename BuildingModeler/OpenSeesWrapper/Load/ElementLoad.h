#pragma once

#include "Load.h"

namespace opensees
{
	class ElementLoad : public Load
	{
	private:
		int m_elementTag;

	public:
		ElementLoad(int elementTag, std::vector<double> loadVector);
		ElementLoad() = delete;
		~ElementLoad() {}

		int getElementTag() const;
		std::string getOpenseesCommand() const override;
	};
}