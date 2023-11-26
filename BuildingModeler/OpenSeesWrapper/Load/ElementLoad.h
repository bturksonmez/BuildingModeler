#pragma once

#include "Load.h"

namespace opensees
{
	class ElementLoad : Load
	{
	private:
		int m_elementTag;

		ElementLoad(int elementTag, std::vector<double> loadVector);

	public:
		ElementLoad() = delete;
		~ElementLoad() {}

		int getElementTag() const;
		std::string getOpenseesCommand() const override;
	};
}