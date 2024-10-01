#pragma once

#include <memory>
#include <nlohmann/json.hpp>

namespace buildingModeler
{
	class BuildingModelerAPI;
}

namespace buildingGenerator
{
	struct Parameters {
		int minNumberOfBays;
		int maxNumberOfBays;
		double minBayWidth;
		double maxBayWidth;
		int minNumberOfStoreys;
		int maxNumberOfStoreys;
		double minFirstStoreyHeight;
		double maxFirstStoreyHeight;
		double minStoreyHeight;
		double maxStoreyHeight;
		double maxAspectRatioForColumns;
		double maxAreaRatioInnerToOuterColumns;
		double minEquivalentSquareColumnWidth;
		double maxEquivalentSquareColumnWidth;
		double minBeamWidth;
		double maxBeamWidth;
		double minBeamDepth;
		double maxBeamDepth;
		double minShearWallThickness;
		double maxShearWallThickness;
		double minSlabThickness;
		double maxSlabThickness;
		double minConcreteYoungsModulus;
		double maxConcreteYoungsModulus;
	};

	typedef nlohmann::json json;

	class IBuildingGenerator
	{
	protected:
		typedef buildingModeler::BuildingModelerAPI api;

	public:
		virtual json generate() = 0;
		virtual ~IBuildingGenerator() = default;

		template<typename T, typename... Args>
		static std::unique_ptr<IBuildingGenerator> create(const Parameters& params, Args&&... args) {
			return std::make_unique<T>(params, std::forward<Args>(args)...);
		}
	};
}