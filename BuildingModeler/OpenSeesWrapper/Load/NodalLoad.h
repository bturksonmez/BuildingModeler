#pragma once

#include "Load.h"

namespace opensees
{
	class NodalLoad : public Load
	{
	private:
		int m_nodeTag;

	public:
		NodalLoad(int nodeTag, std::vector<double> loadVector);
		NodalLoad() = delete;
		~NodalLoad() {}

		int getNodeTag() const;
		std::string getOpenseesCommand() const override;
	};
}

