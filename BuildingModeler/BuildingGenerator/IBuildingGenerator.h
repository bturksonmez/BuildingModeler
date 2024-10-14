#pragma once

#include "../LoadingGenerator/ILoadingGenerator.h"
#include <memory>
#include <nlohmann/json.hpp>

namespace buildingModeler
{
	class BuildingModelerAPI;
}

namespace buildingGenerator
{
	struct GeometricParameters {
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
	};

	struct BeamParameters {
		double minBeamWidth;
		double maxBeamWidth;
		double minBeamDepth;
		double maxBeamDepth;
		double minBeamCrackedSectionModifier;
		double maxBeamCrackedSectionModifier;
	};

	struct ColumnParameters {
		double maxAspectRatioForColumns;
		double maxAreaRatioInnerToOuterColumns;
		double minEquivalentSquareColumnWidth;
		double maxEquivalentSquareColumnWidth;
		double minColumnCrackedSectionModifier;
		double maxColumnCrackedSectionModifier;
	};

	struct ShearWallParameters {
		bool includeShearWallInXDir;
		bool includeShearWallInYDir;
		double maxShearWallRatio;
		double minShearWallThickness;
		double maxShearWallThickness;
		double minShearWallCrackedSectionModifier;
		double maxShearWallCrackedSectionModifier;
	};

	struct SlabParameters {
		double minSlabThickness;
		double maxSlabThickness;
	};

	struct MeshInfo {
		double meshSensitivity = 1.0;
		bool meshSlabBeam = false;
		bool meshColumn = false;
	};

	struct ModelingPreferences {
		bool includeMassFromMembers = false;
		bool includePDeltaEffects = false;
		bool disableSlabElements = false;
		bool gravityThroughLineElements = true;
		bool makeFloorsRigid = true;
		bool includeDeadLoadFromMembers = true;
		double minLiveLoadPerArea;
		double maxLiveLoadPerArea;
		double minLiveLoadMassContribution;
		double maxLiveLoadMassContribution;
	};

	struct Parameters {
		GeometricParameters geometricParameters;
		ColumnParameters columnParameters;
		BeamParameters beamParameters;
		ShearWallParameters shearWallParameters;
		SlabParameters slabParameters;
		double minConcreteYoungsModulus;
		double maxConcreteYoungsModulus;
		MeshInfo meshInfo;
		ModelingPreferences modelingPreferences;
	};

	typedef nlohmann::json json;

	class IBuildingGenerator
	{
	protected:
		typedef buildingModeler::BuildingModelerAPI api;

	public:
		virtual json generateAndAnalyze() = 0;
		virtual ~IBuildingGenerator() = default;

		template<typename T, typename... Args>
		static std::unique_ptr<IBuildingGenerator> create(const Parameters& params, std::unique_ptr<loadingGenerator::ILoadingGenerator> loading, Args&&... args) {
			return std::make_unique<T>(params, std::move(loading), std::forward<Args>(args)...);
		}
	};
}