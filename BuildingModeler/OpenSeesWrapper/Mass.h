#pragma once

#include "OpenseesObject.h"

namespace opensees
{
	class Mass : public OpenseesObject
	{
	protected:
		int m_nodeTag;
		std::vector<double> m_massValues;

	public:
		Mass(int nodeTag, std::vector<double> massValues);
		Mass() = delete;
		~Mass() {}

		int getNodeTag() const;
		const std::vector<double>& getMassValues() const;

		std::string getOpenseesCommand() const override;
	};
}
