#pragma once

#include "IBuildingGenerator.h"

namespace buildingGenerator
{
	class RegularPlanBuildingGenerator : public IBuildingGenerator
	{
	private:
		Parameters m_parameters;
		bool m_symXDir;
		bool m_symYDir;
		bool m_symBothDir;

	public:
		RegularPlanBuildingGenerator(const Parameters& parameters, bool symmetricInXDir = false, bool symmetricInYDir = false);
		RegularPlanBuildingGenerator() = delete;
		~RegularPlanBuildingGenerator() = default;

		void build() override;
	};
}