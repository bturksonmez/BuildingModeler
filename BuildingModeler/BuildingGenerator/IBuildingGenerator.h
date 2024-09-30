#pragma once

#include <memory>

namespace buildingGenerator
{
	struct Parameters {
		int minNumberOfBays;
		int maxNumberOfBays;
		double minBayWidth;
		double maxBayWidth;
		int minNumberOfStoreys;
		int maxNumberOfStoreys;
		double minFirstFloorHeight;
		double maxFirstFloorHeight;
		double minFloorHeight;
		double maxFloorHeight;
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

	class IBuildingGenerator
	{
	public:
		virtual void build() = 0;
		virtual ~IBuildingGenerator() = default;

		template<typename T, typename... Args>
		static std::unique_ptr<IBuildingGenerator> create(const Parameters& baseParams, Args&&... args) {
			return std::make_unique<T>(baseParams, std::forward<Args>(args)...);
		}
	};
}