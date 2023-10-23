#pragma once

#include "Object.h"

namespace opensees
{
	class Mass : public Object
	{
	protected:
		int m_nodeTag;
		std::vector<double> m_massValues;

	public:
		Mass(int nodeTag, std::vector<double> massValues);
		~Mass() {}

		int getNodeTag() const;
		const std::vector<double>& getMassValues() const;

		std::string getOpenseesCommand() const override;
	};
}
