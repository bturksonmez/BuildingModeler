#pragma once

#include "Load.h"

namespace opensees
{
	class NodalLoad : Load
	{
	private:
		int m_nodeTag;

		NodalLoad(int nodeTag, std::vector<double> loadVector);

	public:
		NodalLoad() = delete;
		~NodalLoad() {}

		int getNodeTag() const;
		std::string getOpenseesCommand() const override;
	};
}

