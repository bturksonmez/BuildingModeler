#include <iostream>

#include "BuildingGenerator/RegularPlanBuildingGenerator.h"
#include "LoadingGenerator/GravityLoadingGenerator.h"
#include "LoadingGenerator/ELFLoadingGenerator.h"
#include "Utilities/VectorUtilities.h"
#include "BuildingModelerAPI.h"

using namespace std;
using namespace buildingGenerator;
using namespace loadingGenerator;

int main()
{
	Parameters params;
	params.geometricParameters.minNumberOfBays = 2;
	params.geometricParameters.maxNumberOfBays = 8;
	params.geometricParameters.minBayWidth = 2.0;
	params.geometricParameters.maxBayWidth = 6.0;
	params.geometricParameters.minNumberOfStoreys = 3;
	params.geometricParameters.maxNumberOfStoreys = 10;
	params.geometricParameters.minFirstStoreyHeight = 2.8;
	params.geometricParameters.maxFirstStoreyHeight = 4.5;
	params.geometricParameters.minStoreyHeight = 2.8;
	params.geometricParameters.maxStoreyHeight = 3.5;
	params.columnParameters.maxAspectRatioForColumns = 1.5;
	params.columnParameters.maxAreaRatioInnerToOuterColumns = 2.0;
	params.columnParameters.minEquivalentSquareColumnWidth = 0.35;
	params.columnParameters.maxEquivalentSquareColumnWidth = 0.60;
	params.columnParameters.minColumnCrackedSectionModifier = 1.0;
	params.columnParameters.maxColumnCrackedSectionModifier = 1.0;
	params.beamParameters.minBeamWidth = 0.25;
	params.beamParameters.maxBeamWidth = 0.40;
	params.beamParameters.minBeamDepth = 0.45;
	params.beamParameters.maxBeamDepth = 0.7;
	params.beamParameters.minBeamCrackedSectionModifier = 1.0;
	params.beamParameters.maxBeamCrackedSectionModifier = 1.0;
	params.shearWallParameters.includeShearWalls = true;
	params.shearWallParameters.maxShearWallRatio = 0.03;
	params.shearWallParameters.minShearWallThickness = 0.25;
	params.shearWallParameters.maxShearWallThickness = 0.3;
	params.shearWallParameters.minShearWallCrackedSectionModifier = 1.0;
	params.shearWallParameters.maxShearWallCrackedSectionModifier = 1.0;
	params.slabParameters.minSlabThickness = 0.15;
	params.slabParameters.maxSlabThickness = 0.20;
	params.minConcreteYoungsModulus = 30000000;
	params.maxConcreteYoungsModulus = 40000000;
	params.meshInfo.meshSensitivity = 0.9;
	params.meshInfo.meshColumn = false;
	params.meshInfo.meshSlabBeam = false;
	params.modelingPreferences.includeMassFromMembers = true;
	params.modelingPreferences.includePDeltaEffects = false;
	params.modelingPreferences.disableSlabElements = true;
	params.modelingPreferences.gravityThroughLineElements = true;
	params.modelingPreferences.makeFloorsRigid = true;
	params.modelingPreferences.includeDeadLoadFromMembers = false;
	params.modelingPreferences.minLiveLoadPerArea = 2.0;
	params.modelingPreferences.maxLiveLoadPerArea = 3.0;
	params.modelingPreferences.minLiveLoadMassContribution = 0.3;
	params.modelingPreferences.maxLiveLoadMassContribution = 1.0;

	// Gravity Loading
	double minDeadLoadFactor = 0.9;
	double maxDeadLoadFactor = 1.4;
	double minLiveLoadFactor = 1.0;
	double maxLiveLoadFactor = 1.6;

	// Earthquake Loading
	double minSpectralAcceleration = 0.4;
	double maxSpectralAcceleration = 2.5;

	auto loading = ILoadingGenerator::create<ELFLoadingGenerator>(minSpectralAcceleration, maxSpectralAcceleration, true);
	auto generator = IBuildingGenerator::create<RegularPlanBuildingGenerator>(params, std::move(loading));

	std::string workingDir = std::filesystem::current_path().string();

	for (int i = 1; i <= 5; ++i) {
	
		auto answer = generator->generateAndAnalyze();

		if (!answer.empty()) {
			std::ofstream file(workingDir + "/data/building_info" + std::to_string(i) + ".json");
			file << answer.dump(4);  // The argument 4 specifies indentation for pretty-printing
			file.close();
		}
		
		//std::ofstream file("building_info" + std::to_string(i) + ".json");
		//file << buildingInfo.dump(4);  // The argument 4 specifies indentation for pretty-printing
		//file.close();
	}

	return 0;
}

