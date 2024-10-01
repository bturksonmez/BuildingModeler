#pragma once

#include <random>

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
		long long m_seed;
		std::default_random_engine m_generator;

		void validateInput();
		void generateBuildingPlan(json& buildingInfo);
		void generateFloors(json& buildingInfo);
		void generateJoints(json& buildingInfo);

	public:
		RegularPlanBuildingGenerator(const Parameters& parameters, bool symmetricInXDir = false, bool symmetricInYDir = false);
		RegularPlanBuildingGenerator() = delete;
		~RegularPlanBuildingGenerator() = default;

		json generate() override;
	};
}